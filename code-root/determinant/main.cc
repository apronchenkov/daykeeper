#include "state.h"
#include "states.h"

#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <ctime>


/**
 * Узнать номер текущей конфигурации.
 */
size_t getNo(const state_t & state) {
	if( !check(state) )
		return (size_t)-1;

	const size_t id = pack(invariant(state));

	size_t l = 0, r = states_number;
	while( l < r ) {
		const size_t c = (l + r) / 2;

		if( states[ c ] < id )
			l = c + 1;
		else
			r = c;
	}

	return l;
}


const state_t step(const state_t & State) {
	if( !check(State) ) {
#ifdef DEBUG
		fprintf(stderr, "Illegal state.\n\n");
#endif
		return State;
	}

	// Определяем стоимость текущей конфигурации
	const size_t No = getNo(State);
	const short Det = costs[ No ];

#ifdef DEBUG
	fprintf(stderr, "> cost = %hd\n", Det);
	fprintf(stderr, "> id = %d\n", No);
	fprintf(stderr, "> equal state = %d%d%d\n"
	                ">               %d%d%d\n"
	                ">               %d%d%d\n", unpack(states[ No ]));
#endif

        // Определяем случайную следующую конфигурацию с такой же стоимостью
	state_t state = State;

	for(size_t i = 0; i < 10000; ++i) {
		const size_t k = rand() % 9;

		if( state.m_1d[ k ] != 0 )
			continue;

		state.m_1d[ k ] = rand() % 9 + 1;

		if( check(state) && costs[ getNo(state) ] == Det )
			return state;
		else
			state.m_1d[ k ] = 0;
	}

	return State;
}


int main() {
	assert( nmax == 3 && NMAX == 9 );

	srand( time(NULL) );

	// Основной цикл
	for(;;) {
		size_t r1, r2, r3;

		if( 3 != scanf("%u%u%u", &r1, &r2, &r3) ) {
			const int __errno = errno;

			if( ferror(stdin) ) {
				fprintf(stderr, "%s\n", strerror(__errno));
				return -1;

			}

			if( !feof(stdin) ) {
				fprintf(stderr, "Unknown input error\n");
				return -2;
			}

			return 0;
		}

                const state_t State = { {r1 / 100 % 10, r1 / 10 % 10, r1 % 10,
		                         r2 / 100 % 10, r2 / 10 % 10, r2 % 10,
		                         r3 / 100 % 10, r3 / 10 % 10, r3 % 10} };

		const state_t state = step(State);

		printf("%u%u%u\n%u%u%u\n%u%u%u\n", state.m_1d[0], state.m_1d[1], state.m_1d[2],
		                                   state.m_1d[3], state.m_1d[4], state.m_1d[5],
		                                   state.m_1d[6], state.m_1d[7], state.m_1d[8]);
#ifdef DEBUG
		printf("\n");
#endif
	}
}
