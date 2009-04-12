#include <ctime>
#include <cstdio>
#include <cstdlib>

inline unsigned int modmul(unsigned int a, unsigned int b, unsigned int m) {
	__asm {
		mov eax, a
		mul b
		div m
		mov eax, edx
	}
}

bool rabin_check_process(unsigned int p, unsigned int a) {
	unsigned int s, t, x;

	if( 0 == p % a )
		return false;	

	for(s = 0, t = p - 1; !(t & 1); ++s, t >>= 1);

	for(x = 1; t; a = modmul(a, a, p), t >>= 1)
		if( t & 1 )
			x = modmul(x, a, p);

	if( 1 == x )
		return true;

	for(; s; --s, x = modmul(x, x, p))
		if( p - 1 == x )
			return true;

	return false;
}

bool rabin_check(unsigned int p) {
	const unsigned int A[] = {2, 3, 5, 7, 11};

	for(size_t i = 0; i < sizeof(A)/sizeof(A[0]); ++i) {
		if( A[i] == p )
			return true;
		if( !rabin_check_process(p, A[i]) )
			return false;
	}
	return true;
}

int main() {

	unsigned int n;
	while( 1 == scanf("%u", &n) ) {
		if( n > 1 && rabin_check(n) ) {
			if( n > 1000000000 )
				printf("probably true\n\n");
			else
				printf("true\n\n");
		} else
			printf("false\n\n");
	}

	return 0;
}
