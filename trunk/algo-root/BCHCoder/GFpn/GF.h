/*
 * Базовые операции для полей GF(p)
 * 
 * gf_add(mod, a, b)
 * gf_sub(mod, a, b)
 * gf_inv(mod, a)
 * gf_div(mod, a, b)
 */
#ifndef __GF_h__
#define __GF_h__ 1

#include <assert.h>

inline unsigned int gf_int(unsigned int mod, int a) {
	if( a >= 0 )
		return a%mod;
	else
		return (mod - (-a)%mod)%mod;
}

inline unsigned int gf_add(unsigned int mod, unsigned int a, unsigned int b) {
	return (a + b)%mod;
}

inline unsigned int gf_sub(unsigned int mod, unsigned int a, unsigned int b) {
	return ((mod - b)%mod + a)%mod;
}

inline unsigned int gf_mul(unsigned int mod, unsigned int a, unsigned int b) {
	return ((unsigned long long)a*b)%mod;
}

inline unsigned int gf_inv(unsigned int mod, unsigned int a) {
	unsigned int u0 = 0, r0 = mod;
	unsigned int u1 = 1, r1 = a;
					
	unsigned int q, r;
	unsigned int u;
	
	while( 1 != r1 ) {

		assert( 0 != r1 );
		
		q = r0 / r1;
		r = r0 % r1;
		
		r0 = r1;
		r1 = r;
					
		u = gf_sub(mod, u0, gf_mul(mod, u1, q));
		
		u0 = u1;
		u1 = u;
	}
	
	return u1;
}

inline unsigned int gf_div(unsigned int mod, unsigned int a, unsigned int b) {
	return gf_mul(mod, a, gf_inv(mod, b));
}

#endif // __GF_h__
