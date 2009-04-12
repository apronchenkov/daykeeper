#ifndef __partition_h__
#define __partition_h__

#include <stdio.h>
#include "type.h"

class partition_t {		// класс для длступа к данным раздела; так же хранит базовые величины для данного раздела.
private:
	FILE	*file;
						// поток соответствующий разделу
	void check_values();
						// функция проверки параметров заголовка
protected:
						// Данные из заголовка раздела
	u16_t	bytes_per_sector;
						// колличество байтов в секторе
	u8_t	sectors_per_cluster;
						// колличество секторов в кластере
	u64_t	total_sectors;
						// колличество секторов в разделе
	u64_t	lcn_of_mft;
						// логический номер кластера для файла $MFT
	u64_t	lcn_of_mft_mirr;
						// логический номер кластера для файла $MFTMirr
	i8_t	clusters_per_mft_record;
						// колличество кластеров в mft-записи
	i8_t	clusters_per_index_block;
						// колличество кластеров в индекс-блоке
						// Пересчитанные величины
	u32_t	bytes_per_cluster;
						// колличество байтов в кластере
	u64_t	total_clusters;
						// колличество кластеров в разделе
	u32_t	bytes_per_mft_record;
						// колличество байт в mft-записи
	u32_t	bytes_per_index_block;
						// колличество байт в индекс-блоке
public:
	partition_t(FILE *file);
	~partition_t();
	
	u16_t	get_bytes_per_sector() const {return bytes_per_sector;}
	u8_t	get_sectors_per_cluster() const {return sectors_per_cluster;}
	u64_t	get_total_sectors() const {return total_sectors;}
	u64_t	get_lcn_of_mft() const {return lcn_of_mft;}
	u64_t	get_lcn_of_mft_mirr() const {return lcn_of_mft_mirr;}
	i8_t	get_clusters_per_mft_record() const {return clusters_per_mft_record;}
	i8_t	get_clusters_per_index_block() const {return clusters_per_index_block;}
	
	u32_t	get_bytes_per_cluster() const {return bytes_per_cluster;}

	u64_t	get_total_clusters() const {return total_clusters;}
	u32_t	get_bytes_per_mft_record() const {return bytes_per_mft_record;}
	u32_t	get_bytes_per_index_block() const {return bytes_per_index_block;}

	void read_cluster(u64_t offset, char *buffer);
						// прочитать одиночный кластер
	void read_clusters(u64_t offset, char *buffer, u64_t count);
						// прочитать последовательность кластеров
};

#endif // __partition_h__
