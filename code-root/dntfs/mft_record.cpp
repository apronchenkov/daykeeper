#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "mft_record.h"
#include "attribute.h"
#include "tables.h"

#include "main.h"

void mft_record_t::mft_record_init(const char *buffer) {
	const static char f_name[] = "mft_record_init()";
						// Псевдо имя функции
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4, IS_MEM} state = INIT;
	bool ok = true;
	
	const u32_t bytes_per_mft_record	= partition->get_bytes_per_mft_record();
						// находим константы
	total_attributes = 0;
						// колличество атрибутог
	if( ok ) {
							// считываем заголовок
		header.offset_to_update_sequence = *(u16_t*)(buffer + mr_offset_to_update_sequence);
		header.size_of_update_sequence = *(u16_t*)(buffer + mr_size_of_update_sequence);
		header.logfile_sequence_number = *(u64_t*)(buffer + mr_logfile_sequence_number);
		header.sequence_number = *(u16_t*)(buffer + mr_sequence_number);
		header.hard_link_count = *(u16_t*)(buffer + mr_hard_link_count);
		header.offset_to_first_attribute = *(u16_t*)(buffer + mr_offset_to_first_attribute);
		header.flags = *(u16_t*)(buffer + mr_flags);
		header.reference_to_base_mft_record = *(u64_t*)(buffer + mr_reference_to_base_mft_record);
		header.next_attribute_ID = *(u16_t*)(buffer + mr_next_attribute_ID);
		header.update_sequence_number = *(u16_t*)(buffer + *(u16_t*)(buffer + mr_offset_to_update_sequence));	
	}

	if( ok ) {
							// проходим по списку атрибутов, считаем, проверяем упорядоченность
		u32_t type = 0;
		u32_t off = header.offset_to_first_attribute;

		while( ok ) {
			u32_t att_record_lenght = *(u32_t*)(buffer + off + ah_attribute_record_lenght);
			
			if( ok ) {
									// проверяем упорядоченность списка атрибутов
				state = IS_STATE1;
				ok = (type <= *(u32_t*)(buffer + off + ah_attribute_type));
			}

			if( ok )
									// считываем тип атрибута
				type = *(u32_t*)(buffer + off + ah_attribute_type);

			if( ok && attrib_end_of_attributes == type )
									// если это специальный атрибут, обрываем поиск
				break;
			
			if( ok ) {
									// смещение не должно быть нулевым
				state = IS_STATE2;
				ok = (0 != att_record_lenght);
			}
			if( ok ) {
									// смещение не должно быть слишком большим
				state = IS_STATE3;
				ok = (att_record_lenght <= bytes_per_mft_record - 2*sizeof(u32_t) - off);
			}
			if( ok ) 
									// пересчитываем смещение
				off += att_record_lenght;

			++total_attributes;
		}
	}
	
	if( ok ) {
							// атрибутов должно быть не пуст
		state = IS_STATE4;
		ok = (total_attributes > 0);
	}
	
	if( ok ) {
							// выделяем память под атрибуты
		state = IS_MEM;
		ok = (NULL != (attributes = (struct attribute_t **)malloc(total_attributes*sizeof(struct attribute_t *))));
	}

	u32_t off;
						// смещение атрибута
	if( ok ) {
							// считываем список атрибутов
		off = header.offset_to_first_attribute;
		u32_t i = 0;

		do {
			try {
				struct attribute_t att;
				attribute_init(&att, buffer + off, num_of_mft_record, off);

				state = IS_MEM;
				ok = (NULL != (attributes[i] = (struct attribute_t *)malloc(sizeof(struct attribute_t))));
				if( ok )
					memcpy(attributes[i], &att, sizeof(struct attribute_t));
			} catch(bool) {
				attributes[i] = NULL;
			}

			off += *(u32_t*)(buffer + off + ah_attribute_record_lenght);
			++i;
		} while( ok && i < total_attributes );
	}

	if( ok )
		return;
	
	switch( state ) {
	case IS_STATE1:
		fprintf(stderr, "%s: mft-запись 0x%llx: список атрибутов не упорядочен.\n",
				f_name, num_of_mft_record);
		break;
	case IS_STATE2:
		fprintf(stderr, "%s: mft-запись 0x%llx: атрибут по смещению 0x%llx имеет нулевой размер.\n",
				f_name, num_of_mft_record, off);
		break;
	case IS_STATE3:
		fprintf(stderr, "%s: mft-запись 0x%llx: атрибут по смещению 0x%llx имеет заведомо слишком большой размер.\n",
				f_name, num_of_mft_record, off);
		break;
	case IS_STATE4:
		fprintf(stderr, "%s: mft-запись 0x%llx: список атрибутов пуст.\n",
				f_name, num_of_mft_record);
		break;
	case IS_MEM: 
		fprintf(stderr, "%s: %s\n", f_name, strerror(errno));
		exit(-1);
	default: assert( 0 );
	}

	throw false;
}

mft_record_t::mft_record_t(u64_t _num_of_mft_record) {
						// инициализация переменных (не нуждающееся в проверках)
	num_of_mft_record = _num_of_mft_record;	
	
	const u32_t bytes_per_mft_record = partition->get_bytes_per_mft_record();
						// константы	
	char buffer[bytes_per_mft_record];
	mft->read_mft_record_data(_num_of_mft_record, buffer);
						// считываем данные записи
	mft->record_correct(buffer, _num_of_mft_record);
						// корректируем файловую запись
	mft_record_init(buffer);
						// считываем заголовок и разбираем список атрибутов
}

mft_record_t::mft_record_t(const char *correct_buffer, u64_t _num_of_mft_record) {
	num_of_mft_record	= _num_of_mft_record;	

	mft_record_init(correct_buffer);
					// считываем заголовок и разбираем поля
}

mft_record_t::~mft_record_t() {
	for(u32_t i = 0; i < total_attributes; ++i) {
		if( NULL != attributes[i] )
			attribute_halt(attributes[i]);
		free(attributes[i]);
	}
	free(attributes);
}
