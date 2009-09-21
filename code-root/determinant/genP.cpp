#include <cstddef>
#include <cstdio>
#include <algorithm>

#include "state.h"
#include "permutation.h"


template<class T> void sort2(T & a, T & b) { if( a > b ) std::swap(a, b); }
template<class T> void sort3(T & a, T & b, T & c) { sort2(a, b); sort2(b, c); sort2(a, b); }



int main() {
	const size_t A[3][3] = {
		{0, 4, 8},
		{1, 5, 6},
		{2, 3, 7}
	};

	const size_t B[3][4] = {
		{0, 5, 7},
		{1, 3, 8},
		{2, 4, 6}
	};

	printf("static const size_t P[ ][ NMAX ] = {\n");

	// Просматриваем множество перестановок
	for(Permutation_t p(NMAX);;) {
		bool f = true;

		// Перестановка должна сохранять положительные комбинации определителя
		for(size_t k = 0; f && k < 3; ++k) {
			size_t s[ 3 ] = { p.comp( A[k][0] ), p.comp( A[k][1] ), p.comp( A[k][2] ) };

			sort3(s[ 0 ], s[ 1 ], s[ 2 ]);

			f &= (
			      !memcmp(s, A[ 0 ], sizeof(s)) ||
			      !memcmp(s, A[ 1 ], sizeof(s)) ||
			      !memcmp(s, A[ 2 ], sizeof(s))
			      );
		}

		// Перестановка должна сохранять отрицательные комбинации определителя
		for(size_t k = 0; f && k < 3; ++k) {
			size_t s[ 3 ] = { p.comp( B[k][0] ), p.comp( B[k][1] ), p.comp( B[k][2] ) };

			sort3(s[ 0 ], s[ 1 ], s[ 2 ]);

			f &= (
			      !memcmp(s, B[ 0 ], sizeof(s)) ||
			      !memcmp(s, B[ 1 ], sizeof(s)) ||
			      !memcmp(s, B[ 2 ], sizeof(s))
			      );
		}

		// Если перестановка сохраняет то, что нужно -- записываем её в файл
		if( f ) {
			printf("\t{");
			for(size_t k = 0; k < NMAX; ++k)
				printf("%u, ", p.comp(k));
			printf("}, \n");
		}

		// Шаг цикла
		if( !p.hasNext() )
			break;
		p = p.next();
	}


	printf("};\n");

	return 0;
}
