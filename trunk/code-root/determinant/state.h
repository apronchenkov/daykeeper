#ifndef __state_h__
#define __state_h___

#include <cstddef>
#include <cstring>
#include <cassert>


const size_t nmax = 3;

const size_t NMAX = 9;


/**
 * Тип описывающий состояние игры.
 */
struct state_t {
	size_t m_1d[ NMAX ];
};


/**
 * Оператор лексикографического сравнения состояний.
 */
inline bool operator == (const state_t & s1, const state_t & s2) {
	return memcmp(&s1, &s2, sizeof(state_t)) == 0 ;
}

/**
 * Оператор лексикографического сравнения состояний.
 */
inline bool operator != (const state_t & s1, const state_t & s2) {
	return !(s1 == s2);
}


/**
 * Оператор лексикографического сравнения состояний.
 */
inline bool operator < (const state_t & s1, const state_t & s2) {
	return memcmp(&s1, &s2, sizeof(state_t)) < 0 ;
}


/**
 * Проверка законности состояния.
 */
inline bool check(const state_t & state) {
	bool used[ NMAX + 1 ];
	memset(used, 0, sizeof(used));

	for(size_t i = 0; i < NMAX; ++i) {
		const size_t j = state.m_1d[ i ];
		if( j == 0 )
			continue;
		if( j > NMAX || used[ j ] )
			return false;
		used[ j ] = true;
	}

	return true;
}

/**
 * Упаковать состояние к 32-х битному формату.
 */
inline size_t pack(const state_t & state) {
	assert( NMAX == 9 );

	if( !check(state) )
		return (size_t)-1;

	size_t pk = 0;
	for(size_t i = 0; i < NMAX; ++i)
		pk = pk * 10 + state.m_1d[ i ];

	return pk;
}

/**
 * Распаковать состояние из 32-х битного формата.
 */
inline state_t unpack(size_t pk) {
	assert( NMAX == 9 );

	state_t state;
	for(size_t i = NMAX - 1; i != (size_t)-1; --i) {
		state.m_1d[ i ] = pk % 10;
		pk /= 10;
	}

	return state;
}

/**
 * Приведение к инвариантному состоянию.
 */
inline const state_t invariant(const state_t & state) {
	assert( nmax == 3 && NMAX == 9 );

	static const size_t P[ 36 ][ NMAX ] = {
		{0, 1, 2, 3, 4, 5, 6, 7, 8, }, 
		{0, 2, 1, 6, 8, 7, 3, 5, 4, }, 
		{0, 3, 6, 1, 4, 7, 2, 5, 8, }, 
		{0, 6, 3, 2, 8, 5, 1, 7, 4, }, 
		{1, 0, 2, 7, 6, 8, 4, 3, 5, }, 
		{1, 2, 0, 4, 5, 3, 7, 8, 6, }, 
		{1, 4, 7, 2, 5, 8, 0, 3, 6, }, 
		{1, 7, 4, 0, 6, 3, 2, 8, 5, }, 
		{2, 0, 1, 5, 3, 4, 8, 6, 7, }, 
		{2, 1, 0, 8, 7, 6, 5, 4, 3, }, 
		{2, 5, 8, 0, 3, 6, 1, 4, 7, }, 
		{2, 8, 5, 1, 7, 4, 0, 6, 3, }, 
		{3, 0, 6, 5, 2, 8, 4, 1, 7, }, 
		{3, 4, 5, 6, 7, 8, 0, 1, 2, }, 
		{3, 5, 4, 0, 2, 1, 6, 8, 7, }, 
		{3, 6, 0, 4, 7, 1, 5, 8, 2, }, 
		{4, 1, 7, 3, 0, 6, 5, 2, 8, }, 
		{4, 3, 5, 1, 0, 2, 7, 6, 8, }, 
		{4, 5, 3, 7, 8, 6, 1, 2, 0, }, 
		{4, 7, 1, 5, 8, 2, 3, 6, 0, }, 
		{5, 2, 8, 4, 1, 7, 3, 0, 6, }, 
		{5, 3, 4, 8, 6, 7, 2, 0, 1, }, 
		{5, 4, 3, 2, 1, 0, 8, 7, 6, }, 
		{5, 8, 2, 3, 6, 0, 4, 7, 1, }, 
		{6, 0, 3, 7, 1, 4, 8, 2, 5, }, 
		{6, 3, 0, 8, 5, 2, 7, 4, 1, }, 
		{6, 7, 8, 0, 1, 2, 3, 4, 5, }, 
		{6, 8, 7, 3, 5, 4, 0, 2, 1, }, 
		{7, 1, 4, 8, 2, 5, 6, 0, 3, }, 
		{7, 4, 1, 6, 3, 0, 8, 5, 2, }, 
		{7, 6, 8, 4, 3, 5, 1, 0, 2, }, 
		{7, 8, 6, 1, 2, 0, 4, 5, 3, }, 
		{8, 2, 5, 6, 0, 3, 7, 1, 4, }, 
		{8, 5, 2, 7, 4, 1, 6, 3, 0, }, 
		{8, 6, 7, 2, 0, 1, 5, 3, 4, }, 
		{8, 7, 6, 5, 4, 3, 2, 1, 0, }, 
	};

	state_t Id = state;

	for(size_t p = 0; p < 36; ++p) {
		bool f = false;

		for(size_t i = 0; i < NMAX; ++i) {
			const size_t x = state.m_1d[ P[p][i] ];

			if( !f ) {
				if( Id.m_1d[ i ] < x )
					break;

				f = (Id.m_1d[ i ] > x);
			}

			if( f )
				Id.m_1d[ i ] = x;
		}
	}
	
	return Id;
}


#endif //_state_h__
