#include "partition.h"
#include "type.h"
#include "tables.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <assert.h>

void partition_t::check_values() {
	const static char f_name[] = "partition_init()";

	if( bytes_per_sector < 8 ) {
							/* это основано на размышлениях о строении массива корректеровки:
							   размер сектора должен быть достаточным, чтобы описать начало
							   заголовка mft-записи и проверочное число. */
		fprintf(stderr, "%s: слишком малый размер сектора NFTS.\n", f_name);
		throw false;
	}

	if( bytes_per_mft_record <= mft_record_size ) {
							/* это основано на строении заголовка mft-записи:
							   размер записи должен быть больше 8 байт. */
		fprintf(stderr, "%s: размер mft-записи должен быть больше %u.\n", f_name, mft_record_size);
		throw false;
	}

	if( 0 != (bytes_per_mft_record%bytes_per_sector) ) {
							/* данное предположение значитально упрощает рассуждения о
							   массиве корректеровки. */
		fprintf(stderr, "%s: размер mft-записи не кратен размеру сектора.\n", f_name);
		throw false;
	}

	if( (u32_t)-1LL == total_clusters ) {
							/* это основано на размышлениях о синхронизации mft-записей
							   в цепочки: кластеры 0x0 и 0xff..ff зарезервированы 
							   (играют особую роль). */
		fprintf(stderr, "%s: слишком много кластеров в разделе.\n", f_name);
		throw false;
	}

	if( lcn_of_mft >= total_clusters ||
		lcn_of_mft_mirr >= total_clusters ||
		lcn_of_mft == lcn_of_mft_mirr
		) {
							/* Файлы MFT и MFTMirr должны находится на разделе и недолжны 
							   совпадать. */
		fprintf(stderr, "%s: ошибка расположения таблиц MFT и MFTMirr.\n", f_name);
		throw false;
	}
}


partition_t::partition_t(FILE *_file) {
						// инициализация базовой структуры
	const static char f_name[] = "partition_init()";
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4} state = INIT;
	bool ok = true;

	char partition_header[partition_header_size];
	
	if( ok ) {
							// проверяем файловый поток на ошибки, запоминаем указатель на файловый поток
		state = IS_STATE1;
		file = _file;
		ok = (0 == ferror(file) && 0 == feof(file));
	}
	if( ok ) {
							// переставляем позицию чтения на начало раздела
		state = IS_STATE2;
		ok = (0 == fseeko(file, 0, SEEK_SET));
	}
	if( ok ) {
							// считываем заголовок раздела
		state = IS_STATE3;
		ok = (1 == fread(partition_header, partition_header_size, 1, file));
	}
	if( ok ) {
							// проверяем магическое число NFTS
		state = IS_STATE4;
		static char ntfs_oem_id[] = {'N', 'T', 'F', 'S', ' ', ' ', ' ', ' '};
		ok = (0 == memcmp(partition_header + ph_oem_id, ntfs_oem_id, sizeof(ntfs_oem_id)));
	}
	if( ok ) {
		bytes_per_sector = *(u16_t*)(partition_header + ph_bytes_per_sector);
		sectors_per_cluster = *(u8_t*)(partition_header + ph_sectors_per_cluster);
		total_sectors = *(i64_t*)(partition_header + ph_total_sectors);
		lcn_of_mft = *(i64_t*)(partition_header + ph_lcn_of_mft);
		lcn_of_mft_mirr = *(i64_t*)(partition_header + ph_lcn_of_mft_mirr);
		clusters_per_mft_record = *(i8_t*)(partition_header + ph_clusters_per_mft_record);
		clusters_per_index_block = *(i8_t*)(partition_header + ph_clusters_per_index_block);
		
		bytes_per_cluster = (u32_t)bytes_per_sector*(u32_t)sectors_per_cluster;
		total_clusters = total_sectors/(i64_t)sectors_per_cluster;
		
							// ссылаюсь на проек ntfslib (на исходник, но не документацию)
		if( clusters_per_mft_record < 0 ) 
			bytes_per_mft_record = (u32_t)1<<(-clusters_per_mft_record);
		else
			bytes_per_mft_record = (u32_t)bytes_per_cluster*(u32_t)clusters_per_mft_record;
							// аналогично
		if( clusters_per_index_block < 0 ) 
			bytes_per_index_block = (u32_t)1<<(-clusters_per_index_block);
		else
			bytes_per_index_block = (u32_t)bytes_per_cluster*(u32_t)clusters_per_index_block;
	}
	if( ok )
		check_values();
	
	if( ok )
		return;
		
	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: ошибка файла раздела: %s\n", f_name, strerror(errno)); break;
	case IS_STATE2: fprintf(stderr, "%s: ошибка переноса позицию чтения: %s\n", f_name, strerror(errno)); break;
	case IS_STATE3: fprintf(stderr, "%s: не могу прочитать заголовок раздела: %s\n", f_name, strerror(errno)); break;
	case IS_STATE4: fprintf(stderr, "%s: файловая система раздела отлична от NTFS.\n", f_name); break;
	default: assert( 0 );
	}
	
	throw false;
}

partition_t::~partition_t() {
						// разрушение базовой структуры
	fclose(file);
}

void partition_t::read_cluster(u64_t offset, char *buffer) {
						// прочитать одиночный кластер
	const static char f_name[] = "read_cluster()";
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4} state = INIT;
	bool ok = true;
	
	if( ok ) {
							// проверяем расположение запрашиваемого кластера
		state = IS_STATE1;
		ok = (offset < total_clusters);
	}
	if( ok ) {
							// переносим позицию чтения раздела на необходимый кластер
		state = IS_STATE2;
		ok = (0 == fseeko(file, offset*(u64_t)bytes_per_cluster, SEEK_SET));
	}
	if( ok ) {
							// считываем кластер
		state = IS_STATE3;
		ok = (1 == fread(buffer, bytes_per_cluster, 1, file));
	}
	if( ok )
		return;
	if( IS_STATE3 == state && 0 != feof(file) )
							// если при чтение неожиданно закончился файл
		state = IS_STATE4;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: кластер 0x%llx: не принадлежит разделу.\n",
				f_name, offset);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: кластер 0x%llx: ошибка переноса позиции чтения: %s\n",
				f_name, offset, strerror(errno));
		break;
	case IS_STATE3: 
		fprintf(stderr, "%s: кластер 0x%llx: ошибка чтения: %s\n",
				f_name, offset, strerror(errno));
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: кластер 0x%llx: неожиданный конец данных файла-устроиства.\n", f_name);
		break;
	default: assert( 0 );
	}
	
	throw false;
}

void partition_t::read_clusters(u64_t offset, char *buffer, u64_t count) {
						// прочитать последовательсность кластеров
	const static char f_name[] = "read_clusters()";
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4} state = INIT;
	bool ok = true;
	
	if( ok ) {
							// проверяем расположение заправшиваемого диапазона кластеров
		state = IS_STATE1;
		ok = (offset < total_clusters && count <= total_clusters - offset);
	}
	if( ok ) {
							// переносим позицию чтения раздела на первый кластер диапазона
		state = IS_STATE2;
		ok = (0 == fseeko(file, offset*(u64_t)bytes_per_cluster, SEEK_SET));
	}
	if( ok ) {
							// считываем данные диапазона кластеров
		state = IS_STATE3;
		ok = (count == fread(buffer, bytes_per_cluster, count, file));
	}

	if( ok ) 
		return;

	if( IS_STATE3 == state && 0 != feof(file) )
							// если неожиданно встречен конец файла-устроиства
		state = IS_STATE4;

	switch( state ) {
	case IS_STATE1: 
		fprintf(stderr, "%s: диапазон кластеров [0x%llx, 0x%llx]: не пренадлежит разделу.\n", 
				f_name, offset, offset + count - 1);
		break;
	case IS_STATE2: 
		fprintf(stderr, "%s: диапазон кластеров [0x%llx, 0x%llx]: ошибка переноса позиции чтения: %s\n", 
				f_name, offset, offset + count - 1, strerror(errno));
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: диапазон кластеров [0x%llx, 0x%llx]: ошибка чтения: %s\n",
				f_name, offset, offset + count - 1, strerror(errno)); 
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: диапазон кластеров [0x%llx, 0x%llx]: неожиданный конец файла-устроиства.\n",
				f_name, offset, offset + count - 1);
		break;
	default: assert( 0 );
	}
	throw false;
}
