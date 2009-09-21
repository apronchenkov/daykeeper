#ifndef __permutation_h__
#define __permutation_h__

#include <cstddef>

#include "exceptions.h"


/**
 * Максимальный размер перестановки.
 */
const size_t PMAX = 9;


/**
 * Структура описывающая перестановку и операции над ней.
 */
class Permutation_t {
	size_t m_size;

	size_t m_comp[ PMAX ];

	
public:

	/**
	 * Конструктор тождественной перестановки.
	 */
	Permutation_t(size_t size = 1) throw(IllegalArgumentException) {
		if( size < 1 || PMAX < size )
			throw IllegalArgumentException("Illegal size of permutation.");		
	
		m_size = size;
		for(size_t i = 0; i < size; ++i)
			m_comp[ i ] = i;
	}

	/**
	 * Конструктор перестановки.
	 */
	Permutation_t(size_t size, const size_t * comp) throw(IllegalArgumentException) {
		if( size < 1 || PMAX < size )
			throw IllegalArgumentException("Illegal size of permutation.");
	
		bool used[ PMAX ];
		memset(used, 0, sizeof(used));

		for(size_t i = 0; i < size; ++i) {
			const size_t j = comp[ i ];

			if( size <= j || used[ j ] )
				throw IllegalArgumentException("Illegal value of permutation.");

			used[ j ] = true;
		}

		m_size = size;
		memcpy(m_comp, comp, size * sizeof(size_t));
	}


	/**
	 * Оператор сравнения перестановок на равенство.
	 */
	bool operator == (const Permutation_t & p) const {
		if( m_size != p.m_size )
			return false;

		return !memcmp(m_comp, p.m_comp, m_size * sizeof(size_t));
	}

	/**
	 * Оператор сравнения перестановок на неравенство.
	 */
	bool operator != (const Permutation_t & p) const {
		return !(*this == p);
	}
	
	/**
	 * Проверить перестановку на тождественность.
	 */
	bool isIdentity() const {
		for(size_t i = 0; i < m_size; ++i) {
			if( m_comp[i] != i )
				return false;
		}
		return true;
	}


	/**
	 * Оператор умножения перестановок.
	 */
	Permutation_t & operator *= (const Permutation_t & p) {
		if( m_size < p.m_size ) {
			for(size_t i = 0; i < m_size; ++i)
				m_comp[ i ] = p.m_comp[ p.m_comp[i] ];
			for(size_t i = m_size; i < p.m_size; ++i)
				m_comp[ i ] = p.m_comp[ i ];
			m_size = p.m_size;

		} else {
			for(size_t i = 0; i < m_size; ++i) {
				if( m_comp[ i ] < p.m_size )
					m_comp[ i ] = p.m_comp[ p.m_comp[i] ];
			}
		}

		return * this;
	}

	/**
	 * Оператор деления перестановок.
	 */
	Permutation_t & operator /= (const Permutation_t & p) {
		return *this *= p.invert();
	}
	

	/**
	 * Оператор инвертирования перестановки.
	 */
	const Permutation_t invert() const {
		Permutation_t t;
		t.m_size = m_size;

		for(size_t i = 0; i < m_size; ++i)
			t.m_comp[ m_comp[i] ] = i;

		return t;
	}


	/**
	 * Узнать, есть ли следующая перестановка?
	 */
	bool hasNext() const {
		for(size_t i = 0; i < m_size; ++i) {
			if( m_comp[i] != m_size - 1 - i )
				return true;
		}
		return false;
	}

	/**
	 * Оператор итерирования (вычисления следующей) перестановки.
	 */
	const Permutation_t next() const {
		size_t i, j;

		for(i = m_size - 2; i != (size_t)-1 && m_comp[ i ] > m_comp[ i + 1 ]; --i);
		if( i == (size_t)-1 )
			return * this;

		for(j = i + 2; j < m_size && m_comp[ j ] > m_comp[ i ]; ++j);
		--j;

		Permutation_t t;
		t.m_size = m_size;

		for(size_t k = 0; k < i; ++k)
			t.m_comp[ k ] = m_comp[ k ]; 
		for(size_t k = i + 1; k < m_size; ++k)
			t.m_comp[ k ] = m_comp[ m_size - k + i ];

		t.m_comp[ i ] = m_comp[ j ];
		t.m_comp[ m_size - j + i ] = m_comp[ i ];
	
		return t;
	}

	/**
	 * Проверка чётности перестановки.
	 */
	size_t inversionsNumber() const {
		size_t inversions = 0;
		
		for(size_t i = 0; i < m_size - 1; ++i) {
			for(size_t j = i + 1; j < m_size; ++j)
				inversions += (m_comp[i] > m_comp[j]) ? 1 : 0;
		}
		
		return inversions;
	}


	size_t size() const {
		return m_size;
	}

	const size_t * comp() const {
		return m_comp;
	}

	const size_t comp(size_t i) const throw(IllegalArgumentException) {
		if( i > m_size )
			throw IllegalArgumentException("Permutation_t::comp(size_t): index out of bound.");
		return m_comp[i];
	}
};


/**
 * Оператор умножения перестановок.
 */
inline const Permutation_t operator * (const Permutation_t & p, const Permutation_t & q) {
	return Permutation_t(p) *= q;
}

/**
 * Оператор деления перестановок.
 */
inline const Permutation_t operator / (const Permutation_t & p, const Permutation_t & q) {
	return Permutation_t(p) /= q;
}

#endif //_permutation_h__
