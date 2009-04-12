/**
@file code.h
@author Пронченков Александр
@version 0.5
@date 05 2006
@brief Объявление кода БЧХ.

В этом файле объявлена структура для работы с кодом БЧХ, а так же базовые операции:
	- объявление кода;
	- удаление кода;
	- кодирование;
	- декодирование.

Для описания кода БЧХ необходимо следующее:
	- порождающий многочлен \f$ g \in GF(2) \f$ (необходимо определить его степень и коэффициенты);
	- поле разложения многочлена \f$ g \f$;
	- примитивный корень порождающего многочлена: \f$ \alpha \f$.
*/

#ifndef __code_h__
#define __code_h__ 1

#include "GF2n.h"
#include "poly2.h"

/** Код БЧХ.
В данной структуре хранится описание кода БЧХ, необходимое для алгоритмов кодирования и декодирования.
*/
struct bch_code_t {
	/** Порождающие многочлен кода. */
	poly2_t g;

	/** Степень порождающего многочлена. */
	unsigned int degg;

	/** Поле разложения порождающего многочлена \f$ g \f$ .*/
	struct gf2m_t *gf2m;

	/** Примитивный корень порождающего многочлена \f$ g \f$ .*/
	el_t alpha;

	/** БЧХ-дистанция кода. \f$ \forall i \in [1, d - 1] : g(\alpha^i) = 0 \f$ */
	unsigned int d;

	/** Количество исправляемых кодом ошибок. \f$ t = \frac{d - 1}{2} \f$ */
	unsigned int t;

	/** Степень кодового многочлена. \f$ n = |GF^*(2^m)| - 1 \f$ */
	/** @bug Возникла проблем в терминологии: в документации n - длинна кода, в программе n -- степень кодового многочлена (то есть, на единицу меньше). */
	unsigned int n;

	/** Степень информационного многочлена. \f$ k = n - deg(g) \f$ */
	unsigned int k;
};

/** Определение нового кода БЧХ.
Создание новой структуры bch_code_t.
@param g порождающий многочлен кода
@param degg \f$ deg(g) \f$
@param p сокращающий многочлен для поля разложения \f$ g \f$
@param alpha примитивный корень многочлена \f$ g \f$
@return указатель на структуру новго кода БЧХ
*/
extern struct bch_code_t * new_bch_code(poly2_t g, unsigned int degg, poly_t p, el_t alpha);

/** Удаление структуры БЧХ кода.
@param bch указатель на удаляемую структуру
*/
extern void delete_bch_code(struct bch_code_t *bch);

/** Кодирование открытого блока.
Преобразование информационного многочлена в кодовый.
@param bch структура кода БЧХ
@param I информационный многочлен
@param F переменная для хранения кодового многочлена
@bug Переменные I и F должны иметь определённый размер и адресуемые ими области данных не должны пересекаться..
*/
extern void bch_encode(const struct bch_code_t *bch, const poly2_t I, poly2_t F);

/** Декодирование кодового блока.
Преобразование кодового многочлена в информационный.
@param bch структура кода БЧХ
@param F кодовый многочлен
@param I переменная для хранения информационного многочлена
@return 0 -- ошибка декодирования, 1 -- декодирование успешно
@bug Переменные I и F должны иметь определённый размер и адресуемые ими области данных не должны пересекаться..
*/
extern int bch_decode(const struct bch_code_t *bch, const poly2_t F, poly2_t I);

#endif // __code_h_	
