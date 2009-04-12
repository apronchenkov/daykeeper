//#include "mft_record.h"
#include "main.h"
#include "attribute.h"
#include "type.h"
#include "tables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <assert.h>

const char* get_attribute_type_name(u32_t attribute_type) {
	static char attribute_type_str[16];

	switch( attribute_type ) {
	case 0x10: return "$STANDART_INFORMATION";
	case 0x20: return "$ATTRIBUTE_LIST";
	case 0x30: return "$FILE_NAME";
	case 0x40: return "$VOLUME_ID (?)";
	case 0x50: return "$SECURITY_DESCRIPTOR";
	case 0x60: return "$VOLUME_NAME";
	case 0x70: return "$VOLUME_INFORMATION";
	case 0x80: return "$DATA";
	case 0x90: return "$INDEX_ROOT";
	case 0xa0: return "$INDEX_ALLOCATION";
	case 0xb0: return "$BITMAP";
	case 0xc0: return "$SYMBOLIC_LINK (?)";
	case 0xd0: return "$EA_INFORMATION";
	case 0xe0: return "$EA";
	case 0xf0: return "$PROPERTY_SET";
	case 0x100: return "$LOGGED_UTILITY_STREAM";
	default: 
		sprintf(attribute_type_str, "0x08%x", attribute_type);
		return attribute_type_str;
	}
}

static void attribute_check_values(const struct attribute_t *at) {
						// дополнительные проверки атрибута
	const static char f_name[] = "attribute_init()";
						// Псевдо имя атрибута
	const u64_t bytes_per_cluster = partition->get_bytes_per_cluster();
						// константы

	if( 1 == at->header.non_resident_flag &&
		at->header.nr.real_size_of_attribute > bytes_per_cluster*(at->header.nr.last_vcn + 1 - at->header.nr.starting_vcn) ) {
							/* Фактически проверяется гипотеза о том, что для нерезидентного
							   атрибута всегда размер выделенной области больше объявленного
							   размера */
		
		fprintf(stderr, "%s: атрибут mft-запись 0x%llx по смещению 0x%llx: область выделенная атрибуту меньше объявленного размера атрибута.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		throw false;
	}
}

static void attribute_init_standart_header(struct attribute_t *at, const char *data) {
						// Считываем базовую часть заголовка атрибута
	const static char f_name[] = "attribute_init()";
						// Псевдоимя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4} state = INIT;
	bool ok = true;
	
						// Считываем базовый заголовок атрибута 
						// (он универсален в не зависимости от резидентности)
	u32_t	attribute_type = *(u32_t*)(data + ah_attribute_type);
	u32_t	attribute_record_lenght = *(u32_t*)(data + ah_attribute_record_lenght);
	u8_t	non_resident_flag = *(u8_t*)(data + ah_non_resident_flag);
	u8_t	name_lenght = *(u8_t*)(data + ah_name_lenght);
	u16_t	offset_to_name = *(u16_t*)(data + ah_offset_to_name);
	u16_t	flags = *(u16_t*)(data + ah_flags);
	u32_t	ID = *(u32_t*)(data + ah_ID);

	if( ok ) {
							// проверяем значение флага "нерезидентности"
		state = IS_STATE1;
		ok = (non_resident_flag <= 1);
	}
	if( ok ) {
							// проверяем значение флага
		state = IS_STATE2;
		ok = (ah_flag_normal == flags ||
			  ah_flag_sparse == flags ||
			  ah_flag_compress == flags ||
			  ah_flag_encrypted == flags
			  );
	}

	if( ok ) {
							// проверяем, факт того, что если атрибут не обычный -- он должен быть нерезидентным
		state = IS_STATE3;
		ok = (0 == flags || 0 != non_resident_flag);
	}

	if( ok && 0 != name_lenght ) {
							// проверяем расположение поля "Имя"
		state = IS_STATE4;
		ok = (
			  offset_to_name >= ((0 == non_resident_flag) ? attribute_resident_header_size : attribute_non_resident_header_size) &&
			  offset_to_name < attribute_record_lenght &&
			  sizeof(c_t)*name_lenght <= attribute_record_lenght - offset_to_name
			  );
	}

	if( ok ) {
							// заполняем базовые поля
		at->header.attribute_type = attribute_type;
		at->header.attribute_record_lenght = attribute_record_lenght;
		at->header.non_resident_flag = non_resident_flag;
		at->header.name_lenght = name_lenght;
		at->header.offset_to_name = offset_to_name;
		at->header.flags = flags;
		at->header.ID = ID;
		memcpy(at->header.attribute_name, data + offset_to_name, sizeof(c_t)*name_lenght);
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: недопустимое значение флага нерезидентности.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: недопустимое значение поля флагов.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: значение поля флагов недопускает резидентности атрибута.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: имя атрибута расположено некорректно.\n", 
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	default: assert( 0 );
	}
	throw false;
}

static void resident_attribute_init(struct attribute_t *at, const char *data) {
						// Считываем поля заголовка резедентного атрибута
	const static char f_name[] = "attribute_init()";
						// Псевдоимя функции
	enum{INIT, IS_STATE1, IS_MEM} state = INIT;
	bool ok = true;
	
	const u32_t	attribute_record_lenght	= at->header.attribute_record_lenght;

	const u32_t	r_lenght_of_attribute = *(u32_t*)(data + ahr_lenght_of_attribute);
	const u16_t	r_offset_to_attribute = *(u16_t*)(data + ahr_offset_to_attribute);
	const u8_t	r_indexed_flag = *(u8_t*)(data + ahr_indexed_flag);
						// считываем поля

	if( ok ) {
						// проверяем расположение атрибута
		state = IS_STATE1;
		ok = (
			  r_offset_to_attribute >= attribute_resident_header_size && 
			  r_offset_to_attribute < attribute_record_lenght &&
			  r_lenght_of_attribute <= attribute_record_lenght - r_offset_to_attribute
			  );
	}
				
	if( ok ) {
							// выделяем память
		state = IS_MEM;
		ok = (NULL != (at->header.r.attribute = (char*)malloc(r_lenght_of_attribute)));
	}

	if( ok ) {
							// сохраняем поля и тело атрибута
		at->header.r.lenght_of_attribute = r_lenght_of_attribute;
		at->header.r.indexed_flag = r_indexed_flag;
		memcpy(at->header.r.attribute, data + r_offset_to_attribute, r_lenght_of_attribute);
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: 
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: тела атрибута расположено некорректно.\n", 
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_MEM: 
		fprintf(stderr, "%s: %s\n", f_name, strerror(errno));
		exit(-1);
	default: assert( 0 );
	}
	
	throw false;
}

static void non_resident_normal_attribute_init(struct attribute_t *at, const char *data_runs) {
	const static char f_name[] = "attribute_init()";
						// Псевдо имя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4, IS_MEM} state = INIT;
	bool ok = true;

	const u64_t total_clusters = partition->get_total_clusters();
	const u32_t max_data_runs_lenght = at->header.attribute_record_lenght - at->header.nr.offset_to_data_runs;
						// вытаскиваем константы

	u32_t bytes_per_runs;
						// размер поля data_runs
	u64_t allocated_size_of_attribute = 0;
						// колличество выделенных кластеров
	u64_t last_vcn;
						// номер последнего кластера выделенного атрибуту (посчитаем его сами)
	u32_t off = 0;
						// смещение в теле атрибута data_runs
	u32_t count_len;
	u32_t offset_len;
						// длинна полей count и offset
	u64_t vcn = at->header.nr.starting_vcn;
						// последовательный номер кластера
	u64_t lcn = 0;
						// текущий логический номер кластера
	u64_t count;
	i64_t offset;
						// вспомогательные переменные под соответствующие поля
	if( ok ) {
							// проверка размера поля
		state = IS_STATE1;
		ok = (0 != max_data_runs_lenght);
	}

	while( ok ) {
							// пробежка по телу поля "data_runs"
		count_len = *(u8_t*)(data_runs + off)&0x0f;
		offset_len = (*(u8_t*)(data_runs + off))>>4;
		off += sizeof(u8_t);
		
		if( 0 == count_len && 0 == offset_len )
			break;
		
		if( ok ) {
								// проверяем значение полей "count_len" и "offset_len"
			state = IS_STATE2;
			ok = (count_len > 0 && count_len <= 8 &&
				  offset_len > 0 && offset_len <= 8);
		}

		if( ok ) {
								// проверяем размера поля
			state = IS_STATE1;
			ok = (off < max_data_runs_lenght - count_len - offset_len - sizeof(u8_t));
		}

		if( ok ){
								// считываем поля "count" и "offset"
			count = 0;
			memcpy(&count, data_runs + off, count_len);
			off += count_len;
			
			if( data_runs[off + offset_len - 1] < 0 ) {
				offset	= -1LL;
				memcpy(&offset, data_runs + off, offset_len);
			} else {
				offset	= 0x0;
				memcpy(&offset, data_runs + off, offset_len);
			}

			off += offset_len;
		}

		if( ok ) {
								// пересчитываем смещение и размер области данных
			lcn += offset;
			vcn += count;
			allocated_size_of_attribute += count;
		}

		if( ok ) {
								// проверяем положение кластеров (должны находиться на разделе)
			state = IS_STATE3;
			ok = (lcn < total_clusters && count <= total_clusters - lcn);
		}
	}

	if( ok ) {
							// вычисляем размер поля data runs и номер последнего кластера
		bytes_per_runs = off;
		last_vcn = vcn - 1;
	}
	if( ok ) {
		state = IS_STATE4;
		ok = (last_vcn == at->header.nr.last_vcn);
	}
	
	if( ok ) {
							// выделяем память
		state = IS_MEM;
		at->header.nr.data_runs = (char*)malloc(bytes_per_runs);
	}

	if( ok ) {
							// заполняем поля
		memcpy(at->header.nr.data_runs, data_runs, bytes_per_runs);
		at->header.nr.__allocated_size_of_attribute = allocated_size_of_attribute;
	}

	if( ok ) 
		return;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: поле \"data runs\" выходит за граници атрубута.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: недопустимое значение длинны в поле \"data runs\" по смещению 0x%llx.\n",
				f_name, at->__num_of_mft_record, at->__offset, at->header.nr.offset_to_data_runs + off - 1);
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: атрибут mft-запись 0x%llx по смещению 0x%llx: данные атрибута выходят за граници раздела.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: атрибут mft-запись 0x%llx по смещению 0x%llx: значение поля \"last vcn\" неверно.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_MEM:
		fprintf(stderr, "%s: %s\n", f_name, strerror(errno));
		exit(-1);
	default: assert( 0 );
	}

	throw false;
}

static void non_resident_sparse_attribute_init(struct attribute_t *at, const char *data_runs) {
	const static char f_name[] = "attribute_init()";
						// Псевдо имя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4, IS_MEM} state = INIT;
	bool ok = true;

	const u64_t total_clusters = partition->get_total_clusters();
	const u32_t max_data_runs_lenght = at->header.attribute_record_lenght - at->header.nr.offset_to_data_runs;
						// вытаскиваем константы

	u32_t bytes_per_runs;
						// размер поля data_runs
	u64_t allocated_size_of_attribute = 0;
						// колличество выделенных кластеров
	u64_t last_vcn;
						// номер последнего кластера выделенного атрибуту (посчитаем его сами)
	u32_t off = 0;
						// смещение в теле атрибута data_runs
	u32_t count_len;
	u32_t offset_len;
						// длинна полей count и offset
	u64_t vcn = at->header.nr.starting_vcn;
						// последовательный номер кластера
	u64_t lcn = 0;
						// текущий логический номер кластера
	u64_t count;
	i64_t offset;
						// вспомогательные переменные под соответствующие поля
	if( ok ) {
							// проверка размера поля
		state = IS_STATE1;
		ok = (0 != max_data_runs_lenght);
	}

	while( ok ) {
							// пробежка по телу поля "data_runs"
		count_len = *(u8_t*)(data_runs + off)&0x0f;
		offset_len = (*(u8_t*)(data_runs + off))>>4;
		off += sizeof(u8_t);
		
		if( 0 == count_len && 0 == offset_len )
			break;
		
		if( ok ) {
								// проверяем значение полей "count_len" и "offset_len"
			state = IS_STATE2;
			ok = (count_len > 0 && count_len <= 8 &&
				  offset_len <= 8);
		}

		if( ok ) {
								// проверяем размера поля
			state = IS_STATE1;
			ok = (off < max_data_runs_lenght - count_len - offset_len - sizeof(u8_t));
		}

		if( ok ){
								// считываем поля "count" и "offset"
			count = 0;
			memcpy(&count, data_runs + off, count_len);
			off += count_len;
			
			if( data_runs[off + offset_len - 1] < 0 ) {
				offset	= -1LL;
				memcpy(&offset, data_runs + off, offset_len);
			} else {
				offset	= 0x0;
				memcpy(&offset, data_runs + off, offset_len);
			}

			off += offset_len;
		}

		if( ok ) {
								// пересчитываем смещение и размер области данных
			lcn += offset;
			vcn += count;
			if( offset > 0 )
				allocated_size_of_attribute += count;
		}

		if( ok ) {
								// проверяем положение кластеров (должны находиться на разделе)
			state = IS_STATE3;
			ok = (lcn < total_clusters && count <= total_clusters - lcn);
		}
	}

	if( ok ) {
							// вычисляем размер поля data runs и номер последнего кластера
		bytes_per_runs = off + 1;
		last_vcn = vcn - 1;
	}
	if( ok ) {
		state = IS_STATE4;
		ok = (last_vcn == at->header.nr.last_vcn);
	}
	
	if( ok ) {
							// выделяем память
		state = IS_MEM;
		at->header.nr.data_runs = (char*)malloc(bytes_per_runs);
	}

	if( ok ) {
							// заполняем поля
		memcpy(at->header.nr.data_runs, data_runs, bytes_per_runs);
		at->header.nr.__allocated_size_of_attribute = allocated_size_of_attribute;
	}

	if( ok ) 
		return;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: поле \"data runs\" выходит за граници атрубута.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: недопустимое значение длинны в поле \"data runs\" по смещению 0x%llx.\n",
				f_name, at->__num_of_mft_record, at->__offset, at->header.nr.offset_to_data_runs + off - 1);
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: атрибут mft-запись 0x%llx по смещению 0x%llx: данные атрибута выходят за граници раздела.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: атрибут mft-запись 0x%llx по смещению 0x%llx: значение поля \"last vcn\" неверно.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	case IS_MEM:
		fprintf(stderr, "%s: %s\n", f_name, strerror(errno));
		exit(-1);
	default: assert( 0 );
	}

	throw false;
}

static void non_resident_compress_attribute_init(struct attribute_t *at, const char *data_runs) {
	const static char f_name[] = "attribute_init()";
						// Псевдо имя функции						// вообще говря мы пока не поддерживаем сжатые или зашифрованные атрибуты
	fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: атрибут сжатый -- НЕПОДДЕРЖИВАЕТСЯ.\n",
			f_name, at->__num_of_mft_record, at->__offset);
	throw false;
}
static void non_resident_encrypted_attribute_init(struct attribute_t *at, const char *data_runs) {
	const static char f_name[] = "attribute_init()";
						// Псевдо имя функции
						// вообще говря мы пока не поддерживаем сжатые или зашифрованные атрибуты
	fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: атрибут зашифрованный -- НЕПОДДЕРЖИВАЕТСЯ.\n",
			f_name, at->__num_of_mft_record, at->__offset);
	throw false;
}

static void non_resident_attribute_init(struct attribute_t *at, const char *data) {
						// Считываем поля нерезидентного атрибута
	const static char f_name[] = "attribute_init()";
						// Псевдоимя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_MEM} state = INIT;
	bool ok = true;
	
	const u32_t	attribute_record_lenght = at->header.attribute_record_lenght;
						// считываем константы
	
	const u64_t nr_starting_vcn = *(u64_t*)(data + ahnr_starting_vcn);
	const u64_t	nr_last_vcn = *(u64_t*)(data + ahnr_last_vcn);
	const u16_t	nr_offset_to_data_runs = *(u16_t*)(data + ahnr_offset_to_data_runs);
	const u16_t	nr_compression_unit_size = *(u16_t*)(data + ahnr_compression_unit_size);
	const u64_t	nr_real_size_of_attribute = *(u64_t*)(data + ahnr_real_size_of_attribute);
						// считываем поля
	if( ok ) {
							// проверяем положение поля "data_runs"
		state = IS_STATE1;
		ok = (
			  nr_offset_to_data_runs >= attribute_non_resident_header_size &&
			  nr_offset_to_data_runs < attribute_record_lenght 
			  );
	}
	if( ok ) {
							// заполняем поля
		at->header.nr.starting_vcn = nr_starting_vcn;
		at->header.nr.last_vcn = nr_last_vcn;
		at->header.nr.offset_to_data_runs = nr_offset_to_data_runs;
		at->header.nr.compression_unit_size = nr_compression_unit_size;
		at->header.nr.real_size_of_attribute = nr_real_size_of_attribute;
	}
	
	if( ok ) {
							// дальнейшее считывание происходит в записимости от значений фалгов
		if( ah_flag_normal == at->header.flags )
			non_resident_normal_attribute_init(at, data + nr_offset_to_data_runs);

		else if( ah_flag_sparse == at->header.flags )
			non_resident_sparse_attribute_init(at, data + nr_offset_to_data_runs);

		else if( ah_flag_compress == at->header.flags )
			non_resident_compress_attribute_init(at, data + nr_offset_to_data_runs);

		else if( ah_flag_encrypted == at->header.flags )
			non_resident_encrypted_attribute_init(at, data + nr_offset_to_data_runs);

		else {assert( 0 );}
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: атрибут mft-записи 0x%llx по смещению 0x%llx: поле \"data runs\" расположено некорректно.\n",
				f_name, at->__num_of_mft_record, at->__offset);
		break;
	default: assert( 0 );
	}

	throw false;
}

void attribute_init(struct attribute_t *at, const char *data, u64_t __num_of_mft_record, u32_t __offset) {
			// Заполняем структуру attribute_t.	
	at->__num_of_mft_record = __num_of_mft_record;
	at->__offset = __offset;

	attribute_init_standart_header(at, data);
						// считываем стандартный заголовок
	if( 0 == at->header.non_resident_flag )
							// в зависимости от типа резидентности считываем данные 
		resident_attribute_init(at, data);
	else
		non_resident_attribute_init(at, data);

	attribute_check_values(at);
						// дополнительные проверки атрибута
}

void attribute_halt(struct attribute_t *at) {
		// разрушаем структуру attribute_t
	if( at->header.non_resident_flag == 0 ) 
		free(at->header.r.attribute);
	else
		free(at->header.nr.data_runs);
}
