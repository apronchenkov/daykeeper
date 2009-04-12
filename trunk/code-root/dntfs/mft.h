#ifndef __mft_h__
#define __mft_h__

#include "type.h"
#include "attribute.h"

class mft_t {
						// структура описывающая таблицу MFT
private:
						// Параметры таблици mft
	u64_t 	number_of_mft_records;
						// колличество записей в таблице
	u64_t 	number_of_mft_clusters;
						// размер таблици в кластерах
	u64_t	*lcn;
						// список кластеров таблици
	void check_values();
						// проверка параметров
	void init_using_data(const class attribute_t *att);
						// инициализация с использованием атрибута $DATA
	void scan_clusters(const attribute_t *at, u64_t clusters_count);
						// сосканировать кластеры из data_runs в список кластеров
protected:
public:
	u64_t get_number_of_mft_records() const {return number_of_mft_records;}
	u64_t get_number_of_mft_clusters() const {return number_of_mft_clusters;}
	
	mft_t();
	~mft_t();
	
	void read_mft_record_data(u64_t record_no, char *buffer);
						// прочитать данные mft-записи
	void record_correct(char *file_record_data, u64_t __num_of_mft_record) const;
						// сделать корретировку mft-записи
};

#endif //__mft_h__
