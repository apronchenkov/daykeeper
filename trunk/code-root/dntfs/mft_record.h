#ifndef __mft_record_h__
#define __mft_record_h__

#include "partition.h"
#include "attribute.h"
#include "type.h"

class mft_record_t {
						// mft-запись
private:
	void mft_record_init(const char *buffer);
						// разобрать заголовок и список атрибутов

protected:
	struct {
							// Заголовок mft-записи
		u16_t offset_to_update_sequence;
							// смещение начала массива корректировки
		u16_t size_of_update_sequence;
							// размер в словах массива корректировки
		u64_t logfile_sequence_number;
							// номер последовательности в файле $LOGFILE
		u16_t sequence_number;
							// идентификационный номер (сколько раз mft-запись была использована)
		u16_t hard_link_count;
							// счётчик ссылок
		u16_t offset_to_first_attribute;
							// смещение первого атрибута
		u16_t flags;
							// флаги состояния
		u64_t reference_to_base_mft_record;
							// ссылка на базовую mft-запись
		u16_t next_attribute_ID;
							// номер идентификации для нового атрибута
		u16_t update_sequence_number;
							// контрольное число mft-записи
	} header;
	
	u64_t num_of_mft_record;
						// номер mft-записи
	u32_t total_attributes;
						// колличиство атрибутов у mft-записи
	struct attribute_t **attributes;
						// список атрибутов
public:
	u16_t get_offset_to_update_sequence() const {return header.offset_to_update_sequence;}
	u16_t get_size_of_update_sequence() const {return header.size_of_update_sequence;}
	u64_t get_logfile_sequence_number() const {return header.logfile_sequence_number;}
	u16_t get_sequence_number() const {return header.sequence_number;}
	u16_t get_hard_link_count() const {return header.hard_link_count;}
	u16_t get_offset_to_first_attribute() const {return header.offset_to_first_attribute;}
	u16_t get_flags() const {return header.flags;}
	u64_t get_reference_to_base_mft_record() const {return header.reference_to_base_mft_record;}
	u16_t get_next_attribute_ID() const {return header.next_attribute_ID;}	
	u16_t get_update_sequence_number() const {return header.update_sequence_number;}

	u64_t get_num_of_mft_record() const {return num_of_mft_record;}
	u32_t get_total_attributes() const {return total_attributes;}

	mft_record_t(u64_t num_of_mft_record);
	mft_record_t(const char *correct_buffer, u64_t _num_of_mft_record);
	~mft_record_t();
	
	const struct attribute_t* get_attribute(u32_t attrib_no) const {
		const static char f_name[] = "get_attribute()";

		if( attrib_no >= total_attributes) {
			fprintf(stderr, "%s: mft-запись 0x%llx: атрибута с номером %u нет.", f_name, attrib_no + 1);
			throw false;
		}

		return attributes[attrib_no];
	}
};

#endif //__mft_record_h__
