#include "partition.h"
#include "mft.h"
#include "mft_record.h"
#include "attribute.h"
#include "tables.h"

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <assert.h>

void name_translate(char *dest, u32_t lenght, c_t const *src) {
						/* Преобразовать ЮНИКОД к однобайтному коду.
						   Символы которые не удаётся преобразовать
						   заменяются на шестнадцатеричную запись 
						   \0xnnnn
						*/
	u32_t i;

	for(i = 0; i < lenght; ++i, ++src) 
		if( 0 == ((*src)&0xff00) ) {
			*dest = (*src)&0xff;
			++dest;
		} else {
			sprintf(dest, "\\0x%04x", *src);
			dest += 7;
		}

	*dest = '\0';
}

void what_attribute_mean(const struct attribute_t *att) {

	char buffer[512];
	u32_t lenght;

	if( 0 == att->header.non_resident_flag ) {
		lenght = min2(sizeof(buffer), att->header.r.lenght_of_attribute);
		memcpy(buffer, att->header.r.attribute, lenght);
	} else 
		return;

	if( attrib_FILE_NAME == att->header.attribute_type && lenght > 0x42 ) {
		char file_name[256*7+1];
		name_translate(file_name, *(buffer + 0x40), (c_t*)(buffer + 0x42));
		fprintf(stdout, "  Область имени: \t\t 0x%02x\n", *(buffer + 0x41));
		fprintf(stdout, "  Имя файла:\t\t\t %s\n", file_name);
	} else {
		return;
	}
}

void fpExit(char const *line) {
	throw true;
}

void fpOpss(char const *line) {
	fprintf(stdout, "Opss...\n");
}

void fpPartitionDescribe(char const *line) {
	enum{INIT, IS_STATE1} state = INIT;
	bool ok = true;
	
	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы

	if( ok ) {
							// структура раздела должна быть инициализированна
		state = IS_STATE1;
		ok = ( NULL != partition );
	}

	if( ok ) {
							// выдаётся вся информация из структуры 
		fprintf(stdout, "Информация о разделе: \n\n");
		fprintf(stdout, "  байтов в секторе:\t\t\t %u\n", partition->get_bytes_per_sector());
		fprintf(stdout, "  секторов в кластере:\t\t\t %u\n", partition->get_sectors_per_cluster());
		fprintf(stdout, "  байтов в кластере:\t\t\t %u\n", partition->get_bytes_per_cluster());
		fprintf(stdout, "\n");

		fprintf(stdout, "  секторов в разделе:\t\t\t %llu\n", partition->get_total_sectors());
		fprintf(stdout, "  кластеров в разделе:\t\t\t %llu\n", partition->get_total_clusters());
		fprintf(stdout, "  байтов в разделе:\t\t\t %llu\n", partition->get_total_clusters()*partition->get_bytes_per_cluster());
		fprintf(stdout, "\n");

		fprintf(stdout, "  первый кластер файла $MFT:\t\t %llu\n", partition->get_lcn_of_mft());
		fprintf(stdout, "  первый кластер файла $MFTMirr:\t %llu\n", partition->get_lcn_of_mft_mirr());
		fprintf(stdout, "\n");

		fprintf(stdout, "  кластеров в mft-записе:\t\t %d\n", partition->get_clusters_per_mft_record());
		fprintf(stdout, "  байтов в mft-записи:\t\t\t %u\n", partition->get_bytes_per_mft_record());
		fprintf(stdout, "  кластеров в индекс блоке:\t\t %d\n", partition->get_clusters_per_index_block());
		fprintf(stdout, "  байтов в индекс-блоке:\t\t %u\n", partition->get_bytes_per_index_block());
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: раздел не открыт.\n", argv_0); break;
	default: assert( 0 );
	}
	throw false;
}

void fpMFTDescribe(char const *line) {
	enum{INIT, IS_STATE1} state = INIT;
	bool ok = true;

	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы

	if( ok ) {
							// структура таблици MFT должна быть открыта
		state = IS_STATE1;
		ok = ( NULL != mft );
	}

	if( ok ) {
		fprintf(stdout, "Информация о таблице MFT:\n\n");
		fprintf(stdout, "  байтов в mft-записе:\t\t\t %u\n", partition->get_bytes_per_mft_record());
		fprintf(stdout, "  колличество mft-записей в таблице:\t %llu\n", mft->get_number_of_mft_records());
		fprintf(stdout, "  размер таблици в кластерах:\t\t %llu\n", mft->get_number_of_mft_clusters());
		fprintf(stdout, "  размер таблици в байта:\t\t %llu\n", mft->get_number_of_mft_clusters()*partition->get_bytes_per_cluster());
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: таблица MFT не открыта.\n", argv_0); break;
	default: assert( 0 );
	}
	throw false;
}

void fpMFTRecordDescribe(char const *line) {
	enum{INIT, IS_STATE1} state = INIT;
	bool ok = true;

	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы
	u32_t att_no;
						// вспомогательная переменная для номера атрибута

	if( ok ) {
							// необходимо, чтобы mft-запись была открыта
		state = IS_STATE1;
		ok = (NULL != mft_record);
	}
	if( ok ) {
							// распечатываем заголовок mft-записи
		fprintf(stdout, "Заголовок mft-записи %llx:\n\n", mft_record->get_num_of_mft_record());

		fprintf(stdout, "  контрольное число:\t\t 0x%04x\n", mft_record->get_update_sequence_number());
		fprintf(stdout, "  номер в $LOGFILE:\t\t %llu\n", mft_record->get_logfile_sequence_number());
		fprintf(stdout, "  ИД номер:\t\t\t %u\n", mft_record->get_sequence_number());
		fprintf(stdout, "  счётчик жёстких ссылок:\t %u\n", mft_record->get_hard_link_count());
		fprintf(stdout, "  флаги состояния:\t\t 0x%04x\n", mft_record->get_flags());
		
		fprintf(stdout, "  номер базовой mft-записи:\t %u\n", 
				mft_reference_record_number(mft_record->get_reference_to_base_mft_record()) );
		fprintf(stdout, "  ИД номер базовой mft-записи:\t %u\n", 
				mft_reference_sequence_number(mft_record->get_reference_to_base_mft_record()) );
		
		fprintf(stdout, "\n");
		fprintf(stdout, "  ИД номер следующего атрибута:\t %u\n", mft_record->get_next_attribute_ID());

		fprintf(stdout, "  колличество атрибутов:\t %u\n", mft_record->get_total_attributes());
	}

	if( ok ) {
							// распечатываем стандартные заголовки mft-записей
		att_no = 0;
		while( att_no < mft_record->get_total_attributes() ) {
			fprintf(stdout, "\n");
			fprintf(stdout, "  Атрибут %d\n", att_no + 1);

								// пытается прочитать атрибут
			
			const class attribute_t *att = mft_record->get_attribute(att_no);

			if( NULL != att ) {
				char attribute_name[256*7 + 1];
				
				fprintf(stdout, "    тип:\t\t\t %s\n", get_attribute_type_name(att->header.attribute_type));

				if( 0 == att->header.name_lenght ) {
					fprintf(stdout, "    безымянный\n");
				} else {
					name_translate(attribute_name, att->header.name_lenght, att->header.attribute_name);
					fprintf(stdout, "    имя:\t\t\t %s\n", attribute_name);
				}				
				fprintf(stdout, "    флаг состояния:\t\t 0x%04x\n", att->header.flags);
				fprintf(stdout, "    ИД:\t\t\t\t %u\n", att->header.ID);
				if( att->header.non_resident_flag )
					fprintf(stdout, "    нерезидентный\n");
				else
					fprintf(stdout, "    резидентный\n");

			} else {
				fprintf(stdout, "    содержит ошибку\n");
			}

			++att_no;
		}
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: mft-запись не открыта.\n", argv_0); break;
	default: assert( 0 );
	}
	throw false;
}

void fpAttributeDescribe(char const *line) {
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4} state = INIT;
	bool ok = true;

	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы
	u32_t att_no;
						// номер атрибута
	class attribute_t const *att;
	if( ok ) {
							// mft-запись должна быть открыта
		state = IS_STATE1;
		ok = ( NULL != mft_record );
	}
	
	if( ok ) {
							// считываем номер необходимого атрибута
		state = IS_STATE2;
		ok = (1 == sscanf(line, "%*s%u", &att_no));
	}
	if( ok ) {
							// номер атрибута должен быть не слишком большим
		state = IS_STATE3;
		ok = (att_no > 0 && att_no <= mft_record->get_total_attributes());
	}

	if( ok ) {
							// атрибут с таким номером должен присутствовать
		state = IS_STATE4;
		ok = (NULL != (att = mft_record->get_attribute(att_no - 1)));
	}
	if( ok ) {
							// выдаём информацию по заголовку атрибута
		fprintf(stdout, "Атрибут %u\n\n", att_no);
	
						// пытается прочитать атрибут
		char attribute_name[256*7 + 1];
	
		fprintf(stdout, "  тип:\t\t\t\t %s\n", get_attribute_type_name(att->header.attribute_type));
		
		if( 0 == att->header.name_lenght ) {
			fprintf(stdout, "  безымянный\n");
		} else {
			name_translate(attribute_name, att->header.name_lenght, att->header.attribute_name);
			fprintf(stdout, "  имя:\t\t\t\t %s\n", attribute_name);
		}
		fprintf(stdout, "  флаг состояния:\t\t 0x%04x\n", att->header.flags);
		fprintf(stdout, "  ИД:\t\t\t\t %u\n", att->header.ID);
		if( att->header.non_resident_flag )
			fprintf(stdout, "  нерезидентный\n");
		else
			fprintf(stdout, "  резидентный\n");
		fprintf(stdout, "\n");

		if( att->header.non_resident_flag ) {
			fprintf(stdout, "  первый виртуальный кластер:\t %llu\n", att->header.nr.starting_vcn);
			fprintf(stdout, "  последний виртуальный кластер: %llu\n", att->header.nr.last_vcn);
			fprintf(stdout, "  размер блока сжатия:\t\t %u\n", att->header.nr.compression_unit_size);
			fprintf(stdout, "  размер атрибута:\t\t %u\n", att->header.nr.real_size_of_attribute);
		} else {
			fprintf(stdout, "  флаг индексирования:\t\t 0x%02x\n", att->header.r.indexed_flag);
			fprintf(stdout, "  размер атрибута:\t\t %u\n", att->header.r.lenght_of_attribute);
		}
		fprintf(stdout, "\n");

		what_attribute_mean(att);
	}
	
	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: mft-запись не открыта.\n", argv_0); break;
	case IS_STATE2: fprintf(stderr, "%s: необходимо указать номер атрибута в mft-записи.\n", argv_0); break;
	case IS_STATE3: fprintf(stderr, "%s: атрибут с таким номером отсутсвует.\n", argv_0); break;
	case IS_STATE4: fprintf(stderr, "%s: запись атрибута содержит ошибку.\n", argv_0); break;
	default: assert( 0 );
	}
	throw false;
}

void fpSelectMftRecord(char const *line) {
	enum{INIT, IS_STATE1, IS_STATE2} state = INIT;
	bool ok = true;

	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы
	u64_t number_of_mft_record;

	if( ok ) {
							// стркутура MFT должна быть открыта
		state = IS_STATE1;
		ok = ( NULL != mft );
	}
	
	if( ok ) {
							// считываем номер необходимой mft-записи
		state = IS_STATE2;
		ok = (1 == sscanf(line, "%*s%llu", &number_of_mft_record));
	}

	if( ok ) {
							// если mft-запись уже открыта -- закроем её
		if( NULL != mft_record )
			delete mft_record;
		mft_record = NULL;
		mft_record = new mft_record_t(number_of_mft_record);
	}

	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: таблица MFT не открыта.\n", argv_0); break;
	case IS_STATE2: fprintf(stderr, "%s: необходимо указать номер mft-записи.\n", argv_0); break;
	default: assert( 0 );
	}
	throw false;
}

void fpAttributeStore(char const *line) {
	enum{INIT, IS_STATE1, IS_STATE2, IS_STATE3, IS_STATE4, IS_STATE5, IS_STATE6, IS_STATE7} state = INIT;
	bool ok = true;
	
	char argv_0[64];
	sscanf(line, "%64s", argv_0);
						// считываем имя подпрограммы
	u32_t att_no;
						// номер атрибута
	class attribute_t const *att;

	char file_name[256];

	FILE *file = NULL;

	if( ok ) {
		state = IS_STATE1;
		ok = ( NULL != mft_record );
	}
	
	if( ok ) {
		state = IS_STATE2;
		ok = (2 == sscanf(line, "%*s%u%s", &att_no, file_name));
	}

	if( ok ) {
		state = IS_STATE3;
		ok = (att_no > 0 && att_no <= mft_record->get_total_attributes());
	}

	if( ok ) {
		state = IS_STATE4;
		ok = (NULL != (att = mft_record->get_attribute(att_no - 1)));
	}

	if( ok ) {
		state = IS_STATE5;
		ok = (NULL != (file = fopen(file_name, "w")));
	}

	if( ok ) {
		if( 0 == att->header.non_resident_flag ) {
			state = IS_STATE6;
			ok = (1 == (fwrite(att->header.r.attribute, att->header.r.lenght_of_attribute, 1, file)));
		} else {
			if( ok ) {
				state = IS_STATE7;
				ok = (0 == att->header.nr.starting_vcn);
			}
			if( ok ) {
				const char *data_runs = att->header.nr.data_runs;
				
				u32_t off = 0;
				u64_t lcn = 0;
				u64_t count;
				i64_t offset;
				
				u64_t clusters_count = att->header.nr.last_vcn + 1;

				while( ok && 0 != clusters_count ) {
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
					
					lcn += offset;
					
					u64_t min = min2(count, clusters_count);
					for(u64_t i = 0; ok && i < min; ++i) {
						char buffer[partition->get_bytes_per_cluster()];
						partition->read_cluster(lcn + i, buffer);
						state = IS_STATE6;
						ok = (1 == fwrite(buffer, sizeof(buffer), 1, file));
					}

					clusters_count -= min;
				}
			}
		}
	}

	if( NULL != file ) 
		fclose(file);
	
	if( ok )
		return;

	switch( state ) {
	case IS_STATE1: fprintf(stderr, "%s: mft-запись не открыта.\n", argv_0); break;
	case IS_STATE2: fprintf(stderr, "%s: необходимо указать номер атрибута и имя файла.\n", argv_0); break;
	case IS_STATE3: fprintf(stderr, "%s: атрибут с таким номером отсутсвует.\n", argv_0); break;
	case IS_STATE4: fprintf(stderr, "%s: запись атрибута содержит ошибку.\n", argv_0); break;
	case IS_STATE5: fprintf(stderr, "%s: ошибка в процессе открытия файла: %s\n", argv_0, strerror(errno)); break;
	case IS_STATE6: fprintf(stderr, "%s: ошибка в процессе записи в файл: %s\n", argv_0, strerror(errno)); break;
	case IS_STATE7: fprintf(stderr, "%s: данный атрибут не является первым в цепочке.\n", argv_0);
	default: assert( 0 );
	}
	throw false;
}

typedef void (*command_func_ptr_t)(const char *line);

const struct {
	char command[64];
	command_func_ptr_t func_ptr;
} command[] = {
	{"desc-partition", &fpPartitionDescribe},
	{"desc-mft", &fpMFTDescribe},
	{"desc-mft-record", &fpMFTRecordDescribe},
	{"desc-attribute", &fpAttributeDescribe},
	{"select-mft-record", &fpSelectMftRecord},
	{"store-attribute", &fpAttributeStore},
	{"exit", &fpExit},
	{"quit", &fpExit},
	{"", &fpOpss},
	{"", NULL}
};

char * promt() {
	static char promt1[64];
	static char promt2[64];

	promt1[0] = '\0';
	promt2[0] = '\0';
	
	if( NULL != partition ) {
		strcat(promt1, "<partition ok>");

		if( NULL != mft ) {
			strcat(promt1, "<mft ok>");
			
			if( NULL != mft_record ) {
				sprintf(promt2, "<mft_record %llu>", mft_record->get_num_of_mft_record());
				strcat(promt1, promt2);
			}
		}
	}
	strcat(promt1, "\n->");
	return promt1;
}

void cli() {
	enum{INIT} state = INIT;
	bool ok = true;

	u32_t i;

	while( ok ) {
		char line[1024];
		char cmd[64];

		fprintf(stdout, "%s", promt());

		fgets(line, sizeof(line), stdin);
		sscanf(line, "%64s", cmd);

		for(i = 0; NULL != command[i].func_ptr && 0 != strcmp(command[i].command, cmd); ++i);

		if( NULL != command[i].func_ptr ) {
			try{
				(*command[i].func_ptr)(line);
				fprintf(stdout, "\n");
			} catch (bool f) {
				if( !f ) 
					fprintf(stdout, "Функция завершилась с ошибкой.\n\n");
				else
					ok = false;
			}
		} else {
			fprintf(stdout, "Команда не найдена.\n\n");
		}
		
	}
}
