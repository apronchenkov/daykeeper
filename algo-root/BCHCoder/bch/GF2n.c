/**
@file GF2n.c
@author Пронченков Александр
@version 0.5
@date 05 2006
@brief Арифметика в конечных полях \f$ GF(2^m) \f$.

В данном файле реализованы функции инициализации структуры поля \f$ GF(2^m) \f$ , для \f$ m \in [1, 31] \f$.
*/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "GF2n.h"
#include "main.h"

#include <assert.h>

/** Произведение двух многочленов по модулю третьего.
@param a многочлен множитель
@param b многочлен множитель
@param m многочлен модуль
@param n степень многочлена m
@return \f$ (a*b)\ mod\ m \f$
@note степень многочленов a и b должна быть меньше степени многочлена m
*/
inline const poly_t poly_mul_mod(poly_t a, poly_t b, poly_t m, u32_t n) {
	poly_t r;
	// n действительно степень многочлена m
	assert( 1 == (m >> n) );
	// степени a и b меньше n
	assert( !(a >> n) && !(b >> n) );
	// выбираем наименьший многочлен из a и b
	if( a > b ) {
		a ^= b;
		b ^= a;
		a ^= b;
	}
	
	// достаточно очевидный алгоритм, умножения столбиком
	r = 0;
	while( a ) {
		if( a & 1 )
			r ^= b;

		b = (b << 1);
		if( b >> n )
			 b ^= m;
		
		a = a >> 1;
	}
	
	return r;
}

/** Заполнить таблицу экспонент и логарифмов.
@param gf2m структура поля с заполненными полями: gf2m->reduction, gf2m->alpha, gf2m->m, gf2m->mul_order -- и должна быть выделена память под таблицы
@return 0 -- произошла ошибка, 1 -- успешно
@note Логарифм 0 -- 0.
*/
static int reset_logexp(struct gf2m_t * gf2m) {
	// константы
	const poly_t p = gf2m->p;
	const u32_t m = gf2m->m;
	const poly_t alpha = (poly_t)gf2m->alpha;
	const u32_t mul_order = gf2m->mul_order; 
	
	poly_t x;
	u32_t i;

	// до-определим дискретный логарифм для 0
	gf2m->log[0] = 0;
	
	// заполняем таблицы
	x = 1;
	i = 0;

	do {
		gf2m->exp[i] = x;
		gf2m->log[x] = i;

		x = poly_mul_mod(x, alpha, p, m);
		++i;

	} while( 1 != x && i < mul_order );

	if( 1 != x || mul_order != i )
		// произошла ошибка: либо сокращающий многочлен приводим над GF(2), либо alpha не являются порождающим элементом мультипликативной группы
		return 0;
	
	return 1;
}

// см. заголовочный файл
struct gf2m_t * new_gf2m(poly_t p, el_t alpha) {
	enum{INIT, IS_MEM, IS_mod, IS_alpha, IS_LOGEXP} state = INIT;
	int ok = 1;
	
	struct gf2m_t * gf2m;
	u32_t m;	
		
	if( ok ) {
		// степень многочлена mod должна принадлежать [1, 31]
		state = IS_mod;

		for(m = 0; 0 != (p >> m); ++m);
		m = m - 1;
		
		ok = (0 < m && m < 32);
	}

	if( ok ) {
		// alpha должен принадлежать полю
		state = IS_alpha;
		ok = (0 == (alpha >> m));
	}
	
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (gf2m = malloc(sizeof(struct gf2m_t))));
	}

	if( ok ) {
		gf2m->m = m;
		gf2m->p = p;
		gf2m->add_order = ((u32_t)1) << m;
		gf2m->mul_order = (((u32_t)1) << m) - 1;
		gf2m->alpha = alpha;
	}
	
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (gf2m->log = malloc(gf2m->add_order * sizeof(u32_t))));
	}
	
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (gf2m->exp = malloc(gf2m->mul_order * sizeof(el_t))));
	}
	
	if( ok ) {
		// заполняем таблицу экспонент и логарифмов
		state = IS_LOGEXP;
		ok = (1 == reset_logexp(gf2m));
	}
	
	if( ok )
		return gf2m;
	
	// обработчик ошибок
	switch( state ) {
	case IS_mod:
		fprintf(stderr, "%s: степень сокращающего многочлен должен быть из [1, 31].\n", argv_0);
		break;

	case IS_alpha:
		fprintf(stderr, "%s: alpha не принадлежит полю GF(2**m).\n", argv_0);
		break;
		
	case IS_LOGEXP:
		fprintf(stderr, "%s: ошибка в параметрах поля.\n", argv_0);
		break;

	case IS_MEM:
		fprintf(stderr, "%s: %s\n", argv_0, strerror(errno));
		break;
	
	default:
		assert( 0 );
	}
	
	exit(-1);
}

// см. заголовочный файл
void delete_gf2m(struct gf2m_t * gf2m) {
	free(gf2m->log);
	free(gf2m->exp);
	free(gf2m);
}
