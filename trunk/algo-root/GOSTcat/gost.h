#ifndef __gost_h__
#define __gost_h__

					/// Беззнаковое 64-битное целое.
typedef unsigned long long u64_t;
					/// Беззнаковое 32-битное целое.
typedef unsigned long	u32_t;
					/// Беззнаковое 16-битное целое.
typedef unsigned short	u16_t;
					/// Беззнаковое 8-битное целое.
typedef unsigned char 	u8_t;
					/// Беззнаковое целое число.
typedef unsigned int    ui_t;
					
enum{
									/// Колличество данных необходимых для ключа (в байтах).
		gost_key_size = 32,
									/// Колличество данных необходимых для таблицы замен (в байтах).
		gost_table_size = 64,
									/// Колличество данных необходимых для задания синхрпосылки (в байтах).
		gost_sync_size = 8,
									/// Колличество данных в имитовставке (в байтах).
		gost_im_size = 8
};

					/// Тип для хранения ключа шифрования.
typedef u8_t	GostKey_t[ gost_key_size ];
					/// Тип для хранения таблицы замен.
typedef u8_t	GostTable_t[ gost_table_size ];

typedef u8_t    GostSync_t[ gost_sync_size ];

typedef u8_t    GostIm_t[ gost_im_size ];

					/// Базовый класс для преобразования gost.
class Gost_t {
							/// Ключ шифрования.
		u32_t key[8];
							/// Расширенная таблица замен.
		u8_t table2[4][256];
							/// Основной шаг преобразования.
		inline void main_transform(u32_t &_N0, u32_t &_N1, u32_t _X) const {
				u32_t S = _N0 + _X;
	
				S = 
						(table2[0][(S >> 0) & 0xff] << 0) |
						(table2[1][(S >> 8) & 0xff] << 8) |
						(table2[2][(S >> 16) & 0xff] << 16) |
						(table2[3][(S >> 24) & 0xff] << 24);
				
				S = (S << 11) | (S >> 21);	
				S = S ^ _N1;
				
				_N1 = _N0;
				_N0 = S;
		}

protected:
							/// Цикл преобразования 32-З.
		inline void _32_enc(u32_t &_N0, u32_t &_N1) const {
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);
				
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);
				
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);
				
				main_transform(_N0, _N1, key[7]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[0]);
				
				_N1 ^= _N0;
				_N0 ^= _N1;
				_N1 ^= _N0;
		}
							/// Цикл преобразования 32-Р.
		inline void _32_dec(u32_t &_N0, u32_t &_N1) const {
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);

				main_transform(_N0, _N1, key[7]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[0]);

				main_transform(_N0, _N1, key[7]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[0]);

				main_transform(_N0, _N1, key[7]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[0]);
				
				_N1 ^= _N0;
				_N0 ^= _N1;
				_N1 ^= _N0;
		}
							/// Цикл преобразования 16-З.
		inline void _16_enc(u32_t &_N0, u32_t &_N1) const {
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);
				
				main_transform(_N0, _N1, key[0]);
				main_transform(_N0, _N1, key[1]);
				main_transform(_N0, _N1, key[2]);
				main_transform(_N0, _N1, key[3]);
				main_transform(_N0, _N1, key[4]);
				main_transform(_N0, _N1, key[5]);
				main_transform(_N0, _N1, key[6]);
				main_transform(_N0, _N1, key[7]);
		}

public:
		Gost_t(const GostKey_t &_key, const GostTable_t &_table) {
				int i, j;

				memcpy(key, _key, gost_key_size);

				u8_t table[ 8 ][ 16 ];
				for(i = 0; i < 8; ++i)
						for(j = 0; j < 16; ++j)
								table[i][j] = (_table[i*8 + j/2] >> ((j%2)*4)) & 0x0f;
				
				for(i = 0; i < 4; ++i)
						for(j = 0; j < 256; ++j)
								table2[i][j] = 
										(table[i*2 + 1][j >> 4] << 4) |
										(table[i*2][j & 0x0f]);
		}

		~Gost_t() {
		}
};

					/// Класс алгоритма гаммирования.
class GostGamma_t : public Gost_t {
		union{
				u8_t S1[ 8 ];
				u32_t  S4[ 2 ];
		};

		u32_t offset;
		
		void iter() {
				S4[0] = S4[0] + (u32_t)0x01010101;
				S4[1] = (S4[1] + (u32_t)(0x01010104 - 1))%((u32_t)0xffffffff) + 1;
				_32_enc(S4[0], S4[1]);
		}
		
public:
		GostGamma_t(const GostKey_t &_key, const GostTable_t &_table, const GostSync_t &_sync): Gost_t(_key, _table) {
				memcpy(S1, _sync, gost_sync_size);
				_32_enc(S4[0], S4[1]);
				offset = 0;
		}

		~GostGamma_t() {
		}

		void encode(const u8_t *_open, u8_t *_close, u32_t _size) {
				if( 0 != offset ) {
						int size = (8 - offset) < _size ? (8 - offset) : _size;
						
						for(int i = 0; i < size; ++i)
								_close[i] = _open[i] ^ S1[offset + i];

						if( 8 == offset ) {
								offset = 0;
								iter();
						} else
								return;

						_open += size;
						_close += size;
						_size -= size;
				}
				
				u32_t  size8 = _size/8;
				int    size = _size%8;
				
				while( size8 ) {
						((u32_t*)_close)[0] = ((u32_t*)_open)[0] ^ S4[0];
						((u32_t*)_close)[1] = ((u32_t*)_open)[1] ^ S4[1];
						iter();
						_open += 8;
						_close += 8;
						size8 -= 1;
				}

				for(int i = 0; i < size; ++i)
						_close[i] = _open[i] ^ S1[i];
				offset = size;
		}
};

class GostImito_t: public Gost_t {
		union{
				u8_t S1[ 8 ];
				u32_t  S4[ 2 ];
		};

		u32_t offset;
		
public:
		GostImito_t(const GostKey_t &_key, const GostTable_t &_table): Gost_t(_key, _table) {
				memset(S1, 0, gost_im_size);
				offset = 0;
		}

		~GostImito_t() {
		}

		void encode(const u8_t *_open, u32_t _size) {
				if( 0 != offset ) {
						int size = (8 - offset) < _size ? (8 - offset) : _size;
						
						for(int i = 0; i < size; ++i)
								S1[offset + i] ^= _open[i];

						if( 8 == offset ) {
								offset = 0;
								_16_enc(S4[0], S4[1]);
						} else
								return;

						_open += size;
						_size -= size;
				}
				
				u32_t  size8 = _size/8;
				int    size = _size%8;
				
				while( size8 ) {
						S4[0] ^= ((u32_t*)_open)[0];
						S4[1] ^= ((u32_t*)_open)[1];
						_16_enc(S4[0], S4[1]);
						_open += 8;
						size8 -= 1;
				}

				for(int i = 0; i < size; ++i)
						S1[i] ^= _open[i];
				offset = size;
		}

		const GostIm_t & get_imito() {
				_16_enc(S4[0], S4[1]);
				return S1;
		}
};

#endif // _gost_h_
