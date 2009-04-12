#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <assert.h>

#include "mft.h"
#include "mft_record.h"
#include "partition.h"
#include "tables.h"

#include "main.h"

void mft_t::check_values() {
			// проверяем значение констант из partition
	return;
}

void mft_t::scan_clusters(const attribute_t *at, u64_t clusters_count) {
	const char *data_runs = at->header.nr.data_runs;

	u32_t off = 0;
	u64_t _lcn = 0;
	u64_t count;
	i64_t offset;
				
	while( 0 != clusters_count ) {
		const u32_t	count_len	= *(u8_t*)(data_runs + off)&0x0f;
		const u32_t	offset_len	= (*(u8_t*)(data_runs + off))>>4;
		off += sizeof(u8_t);

		count = 0;
		memcpy(&count, data_runs + off, count_len);
		off += count_len;
				
		if( data_runs[off + offset_len - 1] < 0 ) {
			offset = -1LL;
			memcpy(&offset, data_runs + off, offset_len);
		} else {
			offset = 0LL;
			memcpy(&offset, data_runs + off, offset_len);
		}
		off += offset_len;

		_lcn += offset;

		u64_t min = min2(count, clusters_count);
		for(u64_t i = 0; i < min; ++i) 
			lcn[number_of_mft_clusters + i] = _lcn + i;

		number_of_mft_clusters += min;
		clusters_count -= min;
	}
}

void mft_t::init_using_data(const class attribute_t *at) {
	const static char f_name[] = "mft_init()";
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_MEM} state = INIT;
	bool ok = true;
	
	const u32_t bytes_per_cluster = partition->get_bytes_per_cluster();
	const u32_t bytes_per_mft_record = partition->get_bytes_per_mft_record();
	
	const u32_t real_size_of_attribute = at->header.nr.real_size_of_attribute;
						// константы
	u32_t num_of_clusters;
						// коллечество кластеров необходимое для хранения $MFT
	
	if( ok ) {
							// необходимо чтобы атрибут был нерезидентным
		state = IS_STATE1;
		ok = (1 == at->header.non_resident_flag);
	}

	if( ok ) {
							// чтобы он был обычным
		state = IS_STATE2;
		ok = (ah_flag_normal == at->header.flags);
	}

	if( ok ) 
							// узнаём колличество mft-записей
		number_of_mft_records = real_size_of_attribute/(u64_t)bytes_per_mft_record;
	

	if( ok ) {
							// размер атрибута должен быть кратен размеру mft-записи
		state = IS_STATE3;
		ok = (0 == real_size_of_attribute%(u64_t)bytes_per_mft_record);
	}
	if( ok ) {
							// расчитываем необходимое колличество кластеров
		num_of_clusters = (real_size_of_attribute + bytes_per_cluster - 1)/bytes_per_cluster;
	}
	if( ok ) {
							// выделяем память
		state = IS_MEM;
		ok = (NULL != (lcn = (u64_t*)malloc(sizeof(u64_t)*num_of_clusters)));
	}

	if( ok ) {
							// копируем список кластеров области данных атрибута
		number_of_mft_clusters = 0;
		scan_clusters(at, num_of_clusters);
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: неименованный атрибут $DATA файла $MFT должен быть нерезидентным.\n", f_name);
		break;
	case IS_STATE2: 
		fprintf(stderr, "%s: неименованный атрибут $DATA файла $MFT не может быть разряженным\\сжатым\\зашифрованным.\n", f_name);
		break;
	case IS_STATE3: 
		fprintf(stderr, "%s: размер файла $MFT должен быть кратер размеру кластера.\n", f_name);
		break;

	case IS_MEM:
		fprintf(stderr, "%s: %s\n", f_name, strerror(errno));
		exit(-1);
	default: assert( 0 );
	}

	throw false;
}

mft_t::mft_t() {
	const static char f_name[] = "mft_init()";
						// Псевдо имя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3} state = INIT;
	bool ok = true;

	const u32_t bytes_per_mft_record = partition->get_bytes_per_mft_record();
	const u32_t bytes_per_cluster = partition->get_bytes_per_cluster();
	const u32_t clusters_per_mft_record = (bytes_per_mft_record + bytes_per_cluster - 1)/bytes_per_cluster;
	const u64_t lcn_of_mft = partition->get_lcn_of_mft();
						// вычисляем константы
	char buffer[bytes_per_cluster*clusters_per_mft_record];
						// буфер под первую mft-запись
	partition->read_clusters(lcn_of_mft, buffer, clusters_per_mft_record);
						// считываем первую mft-запись
	record_correct(buffer, 0);
						// делаем корректеровку
	class mft_record_t mft_record(buffer, 0);
						// распознаём mft-запись
	const u32_t total_attributes = mft_record.get_total_attributes();
						// дополнительная константа
	u32_t att_no = 0;
						// номер атрибута
	class attribute_t const *att;
						// указатель на атрибут
	while( att_no < total_attributes &&
		   ( NULL == (att = mft_record.get_attribute(att_no)) ||
			 attrib_ATTRIBUTE_LIST > att->header.attribute_type
			 )
		   )
								// ищим атрибут $ATTRIBUTE_LIST
		++att_no;
	
	if(	att_no != total_attributes &&
		attrib_ATTRIBUTE_LIST == att->header.attribute_type ) {
							// найден атрибут $ATTRIBUTE_LIST
		const class attribute_t *att_attribute_list = att;

		if( ok ) {
								// инициализация с использованием списка атрибутов
			fprintf(stderr, "%s: поддержка $ATTRIBUTE_LIST пока не реализована.\n", f_name);
			throw false;
		}

	} else {
							// в записи отстутствует атрибута $ATTRIBUTE_LIST
							// ищим безымянный атрибут $DATA

		while( att_no < total_attributes &&
			   ( NULL == (att = mft_record.get_attribute(att_no)) ||
				 attrib_DATA > att->header.attribute_type
				 )
			   )
								// ищим первый атрибут $DATA
			++att_no;

		while( att_no < total_attributes && 
			   ( NULL == (att = mft_record.get_attribute(att_no)) ||
				 ( attrib_DATA == att->header.attribute_type && 0 != att->header.name_lenght )
				 )
			   )
								// ищим первый атрибут $DATA без имени
			++att_no;

		if( ok ) {
								// атрибут $DATA должен существовать
			state = IS_STATE1;
			ok = (att_no < total_attributes &&
				   attrib_DATA == att->header.attribute_type);
		}
		if( ok ) {
								// этот атрибут должен быть нерезидентным
			state = IS_STATE2;
			ok = (1 == att->header.non_resident_flag);
		}
		if( ok ) {
								// этот атрибут должен иметь нулевой starting_vcn
			state = IS_STATE3;
			ok = (0 == att->header.nr.starting_vcn);
		}
		if( ok ) {
								// если нашли, запоминаем найденый атрибут
			const class attribute_t *att_data = att;

			init_using_data(att_data);
								// инициализация с использованием атрибута $DATA
		}
	}

	if( ok )
		return;
		
	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: mft-запись 0x0: отсутствует безымянный атрибут $DATA.\n", f_name);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: mft-запись 0x0: безымянный атрибут $DATA является резидентным.\n", f_name);
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: mft-запись 0x0: безымянный атрибут $DATA не первый в цепочке атрибутов.\n", f_name);
		break;
	default: assert( -1 );
	}

	throw false;
}

mft_t::~mft_t() {
	free(lcn);
}

void mft_t::read_mft_record_data(u64_t record_no, char *buffer) {
						// прочитать файловую файловую запись
	const static char f_name[] = "read_mft_record_data()";
						// псевдо имя функции
	enum{INIT, IS_STATE1} state = INIT;
	bool ok = true;
	
	const u32_t bytes_per_cluster		= partition->get_bytes_per_cluster();
	const u32_t bytes_per_mft_record	= partition->get_bytes_per_mft_record();
			// вычисляем константы

	if( ok ) {
							// проверяем, чтобы запись попала в диапазон существующих mft-записей
		state = IS_STATE1;
		ok = (record_no < number_of_mft_records);
	}
	u64_t cluster_no;
	if( ok ) {
							// считываем данные
		cluster_no = (record_no*(u64_t)bytes_per_mft_record)/bytes_per_cluster;
		u64_t offset = (record_no*(u64_t)bytes_per_mft_record)%bytes_per_cluster;
		u32_t lenght = bytes_per_mft_record;
							// вычисляем начальные величины
		char cluster[bytes_per_cluster];
							// буффер под кластер

		if( 0 != offset ) {
								// если от первого кластера нам нужен только хвост
			partition->read_cluster(lcn[cluster_no], cluster);

			const u32_t size = min2((u64_t)lenght, bytes_per_cluster - offset);
			memcpy(buffer, cluster + offset, size);

			lenght -= size;
			buffer += size;
			++cluster_no;
		}
		
		u32_t cluster_lenght = lenght/bytes_per_cluster;
		u32_t tail_lenght = lenght%bytes_per_cluster;
		
		while( 0 != cluster_lenght ) {
								// считываем центральную часть (целые кластеры)
			partition->read_cluster(lcn[cluster_no], buffer);

			lenght -= bytes_per_cluster;
			cluster_lenght -= 1;
			buffer += bytes_per_cluster;
			++cluster_no;
		}
		
		if( 0 != tail_lenght ) {
								// дочитываем хвост
			partition->read_cluster(lcn[cluster_no], cluster);

			memcpy(buffer, cluster, tail_lenght);
		}
	}
	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: 
		fprintf(stderr, "%s: mft-запись 0x%llx: такой записи нет.\n", f_name, cluster_no);
		break;
	default: assert( 0 );
	}

	throw false;
}

void mft_t::record_correct(char *mft_record_data, u64_t __num_of_mft_record) const {
						// Проверка "FILE"а и возможности исправлений через таблице
						// update_sequence -- если возможно, проводится востановление.
	const static char f_name[] = "mft_record_correct()";
						// псевдо имя функции
	enum{INIT, IS_CHECK_MAGIC, IS_CHECK_OFFSETS, IS_CHECK_UPDATE_SEQ} state = INIT;
	bool ok = true;

	const u32_t bytes_per_mft_record = partition->get_bytes_per_mft_record();
	const u16_t bytes_per_sector = partition->get_bytes_per_sector();
	const u32_t sectors_per_mft_record = bytes_per_mft_record/(u32_t)bytes_per_sector;
						// базовые константы	
	u16_t offset_to_update_sequence;
	u16_t size_of_update_sequence;
						// смещение и размер таблици корректриовки
	if( ok ) {
							// проверяем магическое "FILE" в начале
		const static char mft_record_magic[] = {'F', 'I', 'L', 'E'};
		state = IS_CHECK_MAGIC;
		ok = (0 == memcmp(mft_record_data + mr_magic_number, mft_record_magic, sizeof(mft_record_magic)));
	}
	
	if( ok ) {
							// считываем смещение и размер таблици корректировки
		offset_to_update_sequence = *(u16_t*)(mft_record_data + mr_offset_to_update_sequence);
		size_of_update_sequence = *(u16_t*)(mft_record_data + mr_size_of_update_sequence);
	}
	
	if( ok ) {
							// проверяем положение и размер таблици корректировки
		state = IS_CHECK_OFFSETS;
		ok = (	offset_to_update_sequence >= mft_record_size &&
				offset_to_update_sequence < bytes_per_mft_record &&
				size_of_update_sequence*sizeof(u16_t) <= bytes_per_mft_record - offset_to_update_sequence &&
				size_of_update_sequence <= sectors_per_mft_record + 1 &&
				size_of_update_sequence >= 1
			);
	}

	if( ok ) {
							// проверяем контрольное значение в конце каждого кластера
		state = IS_CHECK_UPDATE_SEQ;
		const u16_t update_sequence_number = *(u16_t*)(mft_record_data + offset_to_update_sequence);
		
		for(u32_t i = 1; ok && i <= sectors_per_mft_record; ++i)
			ok = (update_sequence_number == *(u16_t*)(mft_record_data + i*bytes_per_sector - sizeof(u16_t)));
	}

	if( ok ) {
							// восттанавливаем хвостики секторов из мессива корректеровки
		for(u32_t i = 1; i < size_of_update_sequence; ++i)
			*(u16_t*)(mft_record_data + i*bytes_per_sector - sizeof(u16_t)) = 
				*(u16_t*)(mft_record_data + offset_to_update_sequence + sizeof(u16_t));
	}

	if( ok )
		return;

	switch( state ) {
	case IS_CHECK_MAGIC:
		fprintf(stderr, "%s: mft-запись 0x%llx: отсутствует магическое \"FILE\" в начале.\n",
				f_name, __num_of_mft_record);
		break;
	case IS_CHECK_OFFSETS:
		fprintf(stderr, "%s: mft-запись 0x%llx: некорректны смещение или размер массива корректеровки.\n",
				f_name, __num_of_mft_record);
		break;
	case IS_CHECK_UPDATE_SEQ: 
		fprintf(stderr, "%s: mft-запись 0x%llx: один из секторо содержит неверное контрольное число.\n",
				f_name, __num_of_mft_record);
		break;
	default: assert( 0 );
	}

	throw false;
}

