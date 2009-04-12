/**
@file code.c
@author Пронченков Александр
@version 0.5
@date 06 2006
@brief Определение кода БЧХ.

В данном файле определены основные операции с кодом БЧХ:
	- определение;
	- удаление;
	- кодирование;
	- декодирование;
	- исправление ошибок.
*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>

#include "code.h"
#include "GF2n.h"
#include "main.h"

#include <assert.h>

// максимум двух элементов
#define max(a, b) ((a) > (b) ? (a) : (b))

// см. описание в заголовочном файле
struct bch_code_t * new_bch_code(poly2_t g, unsigned int degg, poly_t p, el_t alpha) {
	enum{INIT, IS_MEM, IS_gf2m, IS_g, IS_d} state = INIT;
	int ok = 1;
	
	// параметры g
	const unsigned int degg_div = degg/size_of_poly2_cell;
	const unsigned int degg_mod = degg%size_of_poly2_cell;
	const unsigned int size_of_g = degg/size_of_poly2_cell + 1;
	
	struct bch_code_t *bch;
	struct gf2m_t *gf2m;
	unsigned int d;
	el_t x, y;

	if( ok ) {
		// определение поля
		state = IS_gf2m;
		gf2m = new_gf2m(p, alpha);
	}

	if( ok ) {
		// степень порождающего многочлена должна лежать в (0, gf2m->mul_order)
		state = IS_g;
		ok = (0 < degg && degg < gf2m->mul_order);
	}
	
	if( ok ) {
		// Старший и младший коэффициенты порождающего многочлена кода должны быть ненулевыми.
		state = IS_g;
		ok = (1 == (g[0]&1) && 1 == (g[degg_div]>>degg_mod)); 
	}
	
	if( ok ) {
		// Рассчитываем БЧХ дистанцию кода
		state = IS_d;
		
		x = 1;
		d = 0;
		
		do {
			++d;
			x = mul_gf2m(gf2m, x, alpha);
			y = calc_value_poly2(g, degg, x, gf2m);
		} while( 0 == y );
		
		// БЧХ дистанция кода должна быть больше 1, т.к. alpha должен быть корнем порождающего многочлена
		ok = (d > 1);
	}
	
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (bch = malloc(sizeof(struct bch_code_t))));
	}
	
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (bch->g = malloc(size_of_g*sizeof(poly2_cell_t))));
	}
	
	if( ok ) {
		memcpy(bch->g, g, size_of_g*sizeof(poly2_cell_t));
		bch->degg = degg;		
		bch->alpha = alpha;

		bch->d = d;
		bch->t = (d - 1)/2;

		bch->gf2m = gf2m;
		bch->n = gf2m->mul_order - 1;
		bch->k = bch->n - bch->degg;
	}
	
	if( ok )
		return bch;
	
	switch( state ) {
	case INIT: assert( 0 );
	
	case IS_gf2m:
		fprintf(stderr, "%s: Ошибка в параметра поля.\n", argv_0);
		break;
	case IS_g:
		fprintf(stderr, "%s: Ошибка в порождающем многочлене.\n", argv_0);
		break;
	case IS_d:
		fprintf(stderr, "%s: alpha не является корнем порождающего многочлена.\n", argv_0);
		break;
	case IS_MEM:
		fprintf(stderr, "%s: %s\n", argv_0, strerror(errno));
		break;
	}
	exit(-1);
}

// см. описание в заголовочном файле
void delete_bch_code(struct bch_code_t *bch) {
	delete_gf2m(bch->gf2m);
	free(bch->g);
	free(bch);
}

/** Шаг 0: расчёт синдромов.
Функция рассчитываем синдромы кодового многочлена:
\f[ S[i] = F(\alpha^i) | i \in [1, d - 1]. \f]
С пересчётом на язык программирования:
\f[ S[i] = F(\alpha^{i + 1}) | i \in [0, d - 2]. \f]
@param bch структура кода БЧХ
@param S массив для синдромов
@param F кодовый многочлен
*/
inline void bch_step_0(const struct bch_code_t *bch, el_t *S, const poly2_t F) {
	const struct gf2m_t *gf2m = bch->gf2m;
	const el_t alpha = bch->alpha;
	
	const unsigned int degF = bch->n;	
	const unsigned int d_1 = bch->d - 1;
	
	unsigned int i;
	el_t x = 1;
	
	for(i = 0; i < d_1; ++i) {
		x = mul_gf2m(gf2m, x, alpha);
		S[i] = calc_value_poly2(F, degF, x, gf2m);
	}
}

/** Шаг 2: построение неувязки.
Функция рассчитываем неувязку:
\f[ \Delta_r = \sum_{j = 0}^{L(r - 1)} \Lambda_{j}^{(r - 1)}S_{r - j} .\f]
С пересчётом на язык программирования:
\f[ delta = \sum_{j = 0}^{l} L[j] S[r - j] .\f]
@param bch структура кода БЧХ
@param S массив синдр
омов
@param r номер очередного синдрома (номер шага)
@param L массив коэффициентов локаторного многочлена (\f$ \Lambda^{(r - 1)} \f$)
@param l степень локаторного многочлена (\f$ L(r - 1) \f$)
@return неувязка (\f$ \Delta_r \f$)
*/
inline el_t bch_step_2(const struct bch_code_t *bch, const el_t *S, unsigned int r, const el_t *L, unsigned int l) {
	const struct gf2m_t *gf2m = bch->gf2m; 

	el_t delta = 0;
	unsigned int i;
	
	for(i = 0; i <= l; ++i)
		delta = add_gf2m(gf2m, delta, mul_gf2m(gf2m, L[i], S[r - i]) );
	return delta;
}

/** Шаг 4: пересчёт многочлена L.
Рассчитывается новые коэффициенты локаторного многочлена:
\f[ \Lambda^{(r)}(x) = \Lambda^{(r - 1)}(x) - \Delta_r x B^{(r - 1)}(x) \quad L(r) = max\{L(r - 1), r - L(r - 1)\}. \f]
С пересчётом на язык программирования:
\f[ L[i] = L[i] - delta B[i - 1], i \in [1, b + 1] \quad l = max\{l, r + 1 - l\}. \f]
@param bch структура кода БЧХ
@param L массив коэффициентов локаторного многочлена (\f$ \Lambda^{(r - 1)} \f$)
@param _l указатель на l: степень локаторного многочлена L (\f$ L(r - 1) \f$)
@param r номер шага
@param delta неувязка (\f$ \Delta_r \f$)
@param B массив коэффициентов вспомогательного многочлена
@param b степень вспомогательного многочлена (\f$ deg B \f$)
*/
inline void bch_step_4(const struct bch_code_t *bch, el_t *L, unsigned int *_l, unsigned int r,  el_t delta, const el_t *B, unsigned int b) {
	
	const struct gf2m_t *gf2m = bch->gf2m;
	const unsigned int l = *_l;

	unsigned int i;

	// разбиваем на два случая: когда степень L меньше B и когда степень L больше B 
	if( l <= b ) {
		
		for(i = 1; i <= l; ++i)
			L[i] = sub_gf2m(gf2m, L[i], mul_gf2m(gf2m, B[i - 1], delta));
		
		for(; i <= b + 1; ++i)
			// @proof т.к. поле имеет характеристику 2, следовательно x == -x
			L[i] = mul_gf2m(gf2m, B[i - 1], delta);	
		
	} else {
		for(i = 1; i <= b + 1; ++i)
			L[i] = sub_gf2m(gf2m, L[i], mul_gf2m(gf2m, B[i - 1], delta));
	}
	
	*_l = max(l, r + 1 - l);	
}

/** Шаг 5: пересчёт многочлена B.
Рассчитываются новые коэффициенты вспомогательного многочлена.
\f[ B^{(r)}(x) = \left\{ \begin{tabular}{l l}$ x{}B^{(r-1)}(x) $ & when $L(r) = L(r - 1)$ \\ $\Delta_r^{-1} \Lambda^{(r-1)}(x)$ & when $L(r) > L(r)$ \end{tabular} \right. \f]
С пересчётом на язык программирования:
\f[ B[i] = \left\{\begin{tabular}{l l}$ B[i - 1] $ & when $l' = l$ \\ $delta^{-1}L'[i]$ & when $l' \not= l$ \end{tabular}\right., \f]
где L' и l' определяют \f$ \Lambda^{(r-1)}\f$ и \f$ L(r-1)  \f$ , а L и l -- \f$ \Lambda^{(r)} \f$ и \f$ L(r) \f$ .
@param bch структура кода БЧХ
@param B массив коэффициентов вспомогательного многочлена
@param _b указатель на b: степень вспомогательного многочлена (\f$ deg B \f$)
@param delta неувязка (\f$ \Delta_r \f$)
@param l степень локаторного многочлена L (\f$ L(r) \f$)
@param L2 массив коэффициентов локаторного многочлена (\f$ \Lambda^{(r - 1)} \f$)
@param l2 степень локаторного многочлена L2 (\f$ L(r - 1) \f$)
*/
inline void bch_step_5(const struct bch_code_t *bch, el_t *B, unsigned int *_b, el_t delta, unsigned int l, const el_t *L2, unsigned int l2) {
	const struct gf2m_t *gf2m = bch->gf2m;
	const unsigned int b = *_b;
	
	unsigned int i;
	el_t delta_1;
	
	if( l == l2 ) {
		// L(r) == L(r - 1)
		for(i = b; i != -1; --i)
			B[i + 1] = B[i];
		B[0] = 0;

		*_b = b + 1;
	} else {
		// L(r) > L(r - 1)
		delta_1 = inv_gf2m(gf2m, delta);
		
		for(i = l2; i != -1; --i) 
			B[i] = mul_gf2m(gf2m, delta_1, L2[i]);
		*_b = l2;
	}
}

/** Заключительный шаг: расчёт локаторов и одновременно исправление ошибок.
Перебором ищутся корни локаторного многочлена \f$ \Lambda(x) \f$ -- локаторы. Локаторов должно быть ровно столько, какова степень локаторного многочлена, и ровно столько, сколько произошло ошибок.

Из теории следует, что если ошибок произошло мало, то локаторный многочлен однозначно определится алгоритмом Берлекэмпа; если ошибок произошло более чем bch->_t, то найденный многочлен может не являться локаторным.
@bug Нет доказательства следующего факта: если найден многочлен \f$ \Lambda(x) \f$ , и все его корни принадлежат \f$ GF^*(2^n) \f$, то это гарантирует, что исправленный таким образом кодовый многочлен будет принадлежать коду.
@param bch структура кода БЧХ
@param F кодовый многочлен
@param L коэффициенты локаторного многочлена (\f$ \Lambda(x) \f$)
@param l степень локаторного многочлена (\f$ L \f$)
*/
inline int bch_step_last( const struct bch_code_t *bch, poly2_t F, const el_t *L, unsigned int l) {
	const struct gf2m_t *gf2m = bch->gf2m;
	const unsigned int n = bch->n;

	unsigned int i = 0, j = 0, k;
	el_t x, y;
	// координаты ошибка
	unsigned int degd, degd_div, degd_mod;

	while( i < l && j <= n ) {

		// подбираем корни
		do {
			x = gf2m->exp[j];
			y = 0;

			k = l;
			do {
				y = mul_gf2m(gf2m, y, x);
				y = add_gf2m(gf2m, y, L[k]);
				--k;
			} while( -1 != k );
			
			++j;
		} while( 0 != y && j <= n );

		if( 0 != y )
			// многочлен L не является локаторным (произошло слишком много ошибок)
			return 0;
		
		// находим позицию с ошибкой
		degd = gf2m->log[ inv_gf2m(gf2m, x) ];
		degd_div = degd/size_of_poly2_cell;
		degd_mod = degd%size_of_poly2_cell;
		F[degd_div] ^= (1<<degd_mod);
		
		++i;
	}
	
	return 1;
}

/** Алгоритм восстановления многочлена кода после ошибок.
В качестве вспомогательного используется алгоритм Берлекэмпа для поиска локаторного многочлена. В документации к проекту присутствует мат. доказательство для данных алгоритмов.

Алгоритм разбит на шаги, часть из которых вынесены в отдельные функции.
@param bch код
@param F кодовый многочлен (с ошибками)
@return 0 -- не удалось исправить ошибки, 1 -- ошибки успешно исправлены
*/
int bch_restore(const struct bch_code_t *bch, poly2_t F) {
	// константы
	const unsigned int d = bch->d;
	const unsigned int t = bch->t;
	
	// S[bch->_d - 1]: список синдромов
	el_t *S;

	// L[bch->_t + 1]: коэффициенты многочлена L
	// @proof локаторный многочлен может иметь степень не более t, если его степень больше, значит произошло слишком много ошибок
	el_t *L;
	// степень многочлена L
	unsigned int l;

	// B[bch->_d]: коэффициенты многочлена B
	// @@proof наибольшая степень многочлена B которая может возникнуть в алгоритме -- это bch->_d - 1, это можно показать индукцией
	el_t *B;
	// степень многочлена B
	unsigned int b;

	// L'[bch->_t + 1]: коэффициенты многочлена L' (копия L с предыдущего шага)
	el_t *L2;
	// степень многочлена L'
	unsigned int l2;
	
	// номер итерации алгоритма
	unsigned int r;
	
	// неувязка (шаг 2)
	el_t delta;
	
	// шаг -1: выделяем память
	if(	NULL == (S = malloc((d - 1) * sizeof(el_t))) ||
	   	NULL == (L = malloc((t + 1) * sizeof(el_t))) ||
		NULL == (B = malloc(d * sizeof(el_t))) ||
		NULL == (L2 = malloc((t + 1) * sizeof(el_t)))
		) goto __mem_fault;
	
	// Шаг 0: расчёт синдромов.
	bch_step_0(bch, S, F);
	
	// Алгоритм Берлекэмпа.
	// Шаг 1: инициализация
	L[0] = 1; l = 0;
	B[0] = 1; b = 0;	
	r = 0;
	
	do {
		// L'(x) = L(x)
		memcpy(L2, L, (l + 1)*sizeof(el_t));
		l2 = l;
			
		// Шаг 2: строим неувязку.	
		delta = bch_step_2(bch, S, r, L, l);
		
		// Шаг 3: D == 0 -- ничего не делаем
		// Шаг 4: D != 0 -- пересчитываем L
		if( 0 != delta ) {
			if( b < t )
				bch_step_4(bch, L, &l, r, delta, B, b);
			else
				// @proof Lambda(r)(x) = Lambda(r - 1)(x) - delta_r*x*B(r-1)(x), но degB = b >= t следовательно deg Lambda(r) > t, в соответствии с алгоритмом, это означает что произошло слишком много ошибок.
				goto __return_0;
		}
		
		// Шаг 5: пересчитываем B.
		bch_step_5(bch, B, &b, delta, l, L2, l2);
		
		// Шаг 6: шаг.
		++r;
	} while( r < d );
	
	// Заключительный шаг: ищем локаторы и исправляем ошибки
	if( bch_step_last(bch, F, L, l) )
		goto __return_1;
	
	goto __return_0;
	
	__return_1:
	free(S);
	free(L);
	free(B);
	free(L2);
	return 1;
	
	__return_0:
	free(S);
	free(L);
	free(B);
	free(L2);
	return 0;
	
	__mem_fault:
	fprintf(stderr, "%s: %s\n", argv_0, strerror(errno));
	exit(-1);
}

// см. описание в заголовочном файле
void bch_encode(const struct bch_code_t *bch, const poly2_t I, poly2_t F) {
	const unsigned int degI = bch->k;
	const unsigned int degg = bch->degg;
	
	mul_poly2(F, I, degI, bch->g, degg);
}

// см. описание в заголовочном файле
int bch_decode(const struct bch_code_t *bch, const poly2_t F, poly2_t I) {
	
	const unsigned int degF = bch->n;
	const unsigned int size_of_F = degF/size_of_poly2_cell + 1;
	
	const unsigned int degg = bch->degg;
	const unsigned int degg_div = degg/size_of_poly2_cell;
	
	unsigned int *F2;
	unsigned int i;
	
	// выделяем память для копии F
	if( NULL == (F2 = malloc(size_of_F*sizeof(poly2_cell_t))) )
		goto __mem_fault;
	// первая попытка декодирования
	__try1:
	memcpy(F2, F, size_of_F*sizeof(poly2_cell_t));
	div_poly2(I, F, degF, bch->g, degg);
	// остаток должен равняться нулю
	for(i = degg_div; i != -1; --i)
		if( 0 != F2[i] )
			goto __try2;
	// декодирование успешно
	goto __return_1;
	// вторая попытка декодирования
	__try2:
	memcpy(F2, F, size_of_F*sizeof(poly2_cell_t));
	// пытаемся исправить ошибки в кодовом многочлене
	if( 0 == bch_restore(bch, F2) )
		goto __return_0;
	// если удалось восстановить -- декодируем повторно
	div_poly2(I, F, degF, bch->g, degg);
	// остаток должен равняться нулю
	for(i = degg_div; i != -1; --i)
		if( 0 != F2[i] )
			goto __return_0;

	goto __return_1;
	// успешное завершение
	__return_1:
	free(F2);
	return 1;
	// не успешное завершение
	__return_0:
	free(F2);
	return 0;
	// ошибка памяти
	__mem_fault:
	fprintf(stderr, "%s: %s\n", argv_0, strerror(errno));
	exit(-1);
}

