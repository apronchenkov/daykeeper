#ifndef __GFpolynom_h__
#define __GFpolynom_h__ 1

#include "GF.h"

#include <assert.h>

#define min(a, b)		( ((a) < (b)) ? (a) : (b) )

typedef unsigned int *poly_t;

inline void poly_add(unsigned int _mod,
	int _degr, poly_t _r,
	int _dega, poly_t _a,
	int _degb, poly_t _b) {

	register int i;

	if( _dega > _degb ) {
		for(i = _degr; i > _dega; --i)
			_r[i] = 0;

		for(; i > _degb; --i)
			_r[i] = _a[i];

		for(; i >= 0; --i)
			_r[i] = gf_add(_mod, _a[i], _b[i]);
	} else {
		for(i = _degr; i > _degb; --i)
			_r[i] = 0;

		for(; i > _dega; --i)
			_r[i] = _b[i];

		for(; i >= 0; --i)
			_r[i] = gf_add(_mod, _a[i], _b[i]);
	}
}

inline void poly_sub(unsigned int _mod,
	int _degr, poly_t _r,
	int _dega, poly_t _a,
	int _degb, poly_t _b) {
		
	register int i;

	if( _dega > _degb ) {
		for(i = _degr; i > _dega; --i)
			_r[i] = 0;

		for(; i > _degb; --i)
			_r[i] = _a[i];

		for(; i >= 0; --i)
			_r[i] = gf_sub(_mod, _a[i], _b[i]);
	} else {
		for(i = _degr; i > _degb; --i)
			_r[i] = 0;

		for(; i > _dega; --i)
			_r[i] = gf_sub(_mod, 0, _b[i]);

		for(; i >= 0; --i)
			_r[i] = gf_sub(_mod, _a[i], _b[i]);
	}
}

inline void poly_mul(unsigned int _mod,
	int _degr, poly_t _r,
	int _dega, poly_t _a,
	int _degb, poly_t _b) {

	register int i;

	unsigned int r[_dega + _degb + 1];
	
	for(i = _degr; i >= 0; --i) 
		r[i] = 0;

	for(i = _dega; i >= 0; --i)
		for(register int j = _degb; j >= 0; --j)
			r[i + j] = gf_add(_mod, r[i + j], gf_mul(_mod, _a[i], _b[j]));
			
	for(i = _degr; i >= 0; --i)
		_r[i] = r[i];
}

inline void poly_divmod(unsigned int _mod, 
	int _degq, poly_t _q, 
	int _degr, poly_t _r, 
	int _dega, poly_t _a, 
	int _degb, poly_t _b) {
	register int i;

	for(i = _dega; i >= 0 && 0 == _a[i]; --i);
	_dega = i;

	for(i = _degb; i >= 0 && 0 == _b[i]; --i);
	_degb = i;

	assert(_degb >= 0);

	if( _dega < _degb ) {
		for(i = _dega + 1; i <= _degr; ++i)
			_r[i] = 0;
			
		for(i = min(_degr, _dega); i >= 0; --i)
			_r[i] = _a[i];
		
		for(i = _degq; i >= 0; --i)
			_q[i] = 0;
			
	} else {
		unsigned int Q[_dega - _degb + 1];
		unsigned int R[_dega + 1];

		const unsigned int inv_b = gf_inv(_mod, _b[_degb]);

		for(i = _dega; i >= 0; --i)
			R[i] = _a[i];		
		
		for(i = _dega; i >= _degb; --i) {

			const unsigned int q = Q[i - _degb] = gf_mul(_mod, inv_b, R[i]);

			for(register int j = 0; j <= _degb;	++j)
				R[i - j] = gf_sub(_mod, R[i - j], gf_mul(_mod, q, _b[_degb - j]));
		}
		
		for(i = _dega - _degb + 1; i <= _degq; ++i)
			_q[i] = 0;
		for(i = min(_degq, _dega - _degb); i >= 0; --i)
			_q[i] = Q[i];
			
		for(i = _degb; i <= _degr; ++i)
			_r[i] = 0;
		for(i = min(_degr, _degb - 1); i >= 0; --i)
			_r[i] = R[i];
	}
}

#undef min

#endif // __GFpolynom_h__
