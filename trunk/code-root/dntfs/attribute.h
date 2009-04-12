#ifndef __attributes_h__
#define __attributes_h__

#include "partition.h"
#include "type.h"

extern const char* get_attribute_type_name(u32_t attribute_type);
extern void what_attribute_mean(const struct attribute_t *att);

class mft_record_t;

struct attribute_t {
						// атрибутов mft-записи

						// вспомогательные данные
	u64_t __num_of_mft_record;
						// номер файловой записи содержащей данный атрибут
	u32_t __offset;
						// смещение данного атрибута в файловой записи
						
	struct {
							// данные заголовка атрибута атрибута
		u32_t attribute_type;
							// тип
		u32_t attribute_record_lenght;
							// длинна атрибута в mft-записи
		u8_t non_resident_flag;
							// флаг нерезидентности
		u8_t name_lenght;
							// длинна имени атрибута
		u16_t offset_to_name;
							// смещение начала имени атрибута
		u16_t flags;
							// флаг состояния 
		u16_t ID;
							// идентификатор атрибута
		c_t	attribute_name[255];
							// имя атрибута
		union {
			struct {
				u32_t lenght_of_attribute;
									// размер тела атрибута
				u16_t offset_to_attribute;
									// смещенение тела атрибута
				u8_t indexed_flag;
									// флаг индексирования
				char *attribute;
									// тело атрибута
			} r;
			struct {
				u64_t starting_vcn;
									// начальный виртуальный кластер
				u64_t last_vcn;
									// заключительный кластер
				u16_t offset_to_data_runs;
									// смещение области Адресации
				u16_t compression_unit_size;
									// размер блока сжатия
				u64_t real_size_of_attribute;
									// колличество памяти занимаемое Телом атрибута
				char *data_runs;
									// область vcn
				u64_t __allocated_size_of_attribute;
									// колличество кластеров выделенное под тело атрибута (посчитанно самостоятельно)
			} nr;
		};
	} header;
};

void attribute_init(struct attribute_t *attribute, const char *attrib_data, u64_t __num_of_mft_record, u32_t __offset);
					// наполняем структуру attribute_t
void attribute_halt(struct attribute_t *attribute);
					// разрушаем структуру attribute_t
		
#endif //__attributes_h__
