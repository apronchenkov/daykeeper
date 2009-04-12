#ifndef __data_runs_h__
#define __data_runs_h__

#include "main.h"

typedef bool (*data_runs_func_t)(u64_t offset, u64_t count, void *struct);

inline void data_runs_process(char *data_runs, u32_t data_runs_length, data_runs_func_t func) {
	static const char f_name[] = "data_runs_process()";
						// псевдо имя функции
	enum{INIT, IS_LEN, IS_LEN_FIELD, IS_OFF_CNT, IS_VCN} state = INIT;
	bool ok = true;

	const u64_t total_clusters = partition->get_total_clusters();
						// выписываем необходимые константы
	if( ok ) {
							// проверяем начальную длинну
		state = IS_LEN;
		ok = (data_runs_length > 0);
	}
	
	u64_t vcn = 0;
						// текущий кластер
	u32_t off = 0;
						// смещение в теле поля
	u32_t count_len;
	u32_t offset_len;
						// длинна полей count и offset
	u64_t count;
	i64_t offset;
						// вспомогательные поля под соответствующие величины
	while( ok ) {
							// основной цикл
		count_len = (*(u8_t*)(data + off)) & 0x0f;
		offset_len = (*(u8_t*)(data + off)) >> 4;
		off += sizeof(u8_t);
							// считываем поле длинны
		if( ok && 0 == count_len && 0 == offset_len )
								// проверка на выход
			break;
		
		if( ok ) {
								// проверяем значение полей "count_len" и "offset_len"
			state = IS_LEN_FIELD;
			ok = (count_len <= 8 && offset_len <= 8);
		}

		if( ok ) {
								// проверяем размер поля data_runs
			state = IS_LEN;
			ok = (off < data_runs_length - count_len - offset_len - sizeof(u8_t) );
		}

		if( ok ) {
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
								// проверяем положение кластеров (должны находиться на разделе)
			vcn += offset;
			state = IS_VCN;
			ok = (vcn < total_clusters && count <= total_clusters - vcn);
		}

}

#endif //__data_runs_h__
