/**
@file poly2.h
@author Пронченков Александр
@version 0.1
@date 06 2006
@brief Определение операций над многочленами большой степени.

Объявление типов и функций для работы с многочленами произвольной степени над \f$ GF(2) \f$. Нам понадобятся следующие операции:
	- умножение многочлена на фиксированный многочлен;
	- деление многочлена на фиксированный многочлен;
	- вычисление значения многочлена над полем GF(2^n).
	
Многочлена представляются векторами их коэффициентов (битов), которые хранятся в ячейках (poly2_cell_t) по несколько коэффициентов в каждой (size_of_poly2_cell). 

@bug Реализация данных операций не проверяет всех возможных граничных условий и заточена под конкретное применение в кодах БЧХ. Тем не менее мы стремимся к доказательности нашего кода. Основные границы применимости данных алгоритмов: степень операнда и результата не должны превышать \f$ 2^31 - 1 \f$ , при этом ошибки могут и не возникнуть; области данных аргументов функций не должны пересекаться, т. е. нельзя умножить a на b и результат положить вновь в a; третья потенциальная ошибка -- нет проверки размеров областей данных переменных -- алгоритмы "доверяют" программисту.
*/

#ifndef __poly2_h__
#define __poly2_h__

/** Ячейки многочлена.
*/
typedef unsigned int poly2_cell_t;

/** Тип для многочленов над полем \f$ GF(2) \f$ большой степени.
Для произвольного многочлена \f$ g \f$ выделяются следующие параметры:
	- degg	- степень многочлена (\f$ degg = deg g \f$)
	- degg_div	- степень многочлена в ячейках (\f$ degg\_div = \lfloor\frac{deg g}{size\_of\_poly2\_cell}\rfloor \f$)
	- degg_mod - остаточная степень многочлена (\f$ degg\_mod = deg g\ mod\ size\_of\_poly2\_cell \f$)
	- size_of_g	- количество ячеек занимаемое многочленом g (\f$ size\_of\_g = \lfloor\frac{deg g}{size\_of\_poly2\_cell}\rfloor + 1 \f$)
*/
typedef poly2_cell_t * poly2_t;

/** Количество коэффициентов в каждой poly2_cell_t.
*/
#define size_of_poly2_cell (sizeof(unsigned int)*8)

/** Вычислить значение многочлена большой степени над полем gf2m.
@param f многочлен, значение которого необходимо вычислить
@param degf степень многочлена f
@param x точка в которой необходимо вычислить x
@param gf2m поле над которым рассчитывается результат
@return значение многочлена в точке x
*/
inline el_t calc_value_poly2(const poly2_t f, unsigned int degf, el_t x, const struct gf2m_t *gf2m) {}

/** Произведение двух многочленов больших степеней
@param f переменная для результата (должна иметь достаточный размер)
@param g многочлен множитель
@param degg степень многочлена g
@param h многочлен множитель
@param degh степень многочлена h
*/
inline void mul_poly2(poly2_t f, const poly2_t g, unsigned int degg, const poly2_t h, unsigned int degh) {
	/*poly2_t const a;
	unsigned int dega, dega_div, dega_mod;

	poly2_t const b;
	unsigned int degb, degb_div, degb_mod;
	
	unsigned int size_of_f = (degh + degg)/size_of_poly2_cell;
			
	unsigned int i, j, k;
	
	memset(f, 0, size_of_f*sizeof(poly2_cell_t));

	if( degg < degh ) {
		a = g;
		dega = degg;
		b = h;
		degb = degh;
	} else {
		a = h;
		dega = degh;
		b = g;
		degb = degg;
	}
	
	dega_div = dega/size_of_poly2_cell;
	dega_mod = dega%size_of_poly2_cell;
	
	degb_div = degb/size_of_poly2_cell;
	degb_mod = degb%size_of_poly2_cell;
	
	for(i = 0; i < dega_div; ++i)
		for(j = 0; j < size_of_poly2_cell; ++j) {
			for(k = 0; k < 
		}
	for(i = 0; i < dega_mod; ++i) {
	}
	*/
}

/** Деление с остатком двух многочленов больших степеней
@param q переменная для частного (должна иметь достаточный размер)
@param r многочлен делимое (значение остатка помещается в эту же переменную)
@param degr степень многочлена r
@param g многочлен делитель
@param degg степень многочлена g в ячейках
*/
inline void div_poly2(poly2_t q, poly2_t r, unsigned int degr, const poly2_t g, unsigned int degg) {}

#endif //__poly2_h__

