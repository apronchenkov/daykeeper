#include "exceptions.h"
#include "permutation.h"
#include "state.h"

#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cerrno>
#include <cassert>

#include <set>
#include <map>
#include <algorithm>


typedef std::map<size_t, int> container_t;

container_t States;


void genStates() {
	assert( nmax == 3 && NMAX == 9 && PMAX >= NMAX );
	
	// Фронт рассчётов
	container_t f, F;

	// Вспомогательная переменная
	state_t state;
	container_t::const_iterator i, j;
	size_t k, z = 0;

	// Заполняем фронт соответствующий 9-му шагу.
	for(Permutation_t p(NMAX);;) {
		for(k = 0; k < NMAX; ++k)
			state.m_1d[ k ] = p.comp(k) + 1;

		// Определяем стоимость конфигурации
		const size_t id = pack(invariant(state));

		// Определяем стоимость конфигурации как значение определителя
		const int det = state.m_1d[0] * state.m_1d[4] * state.m_1d[8]
		              + state.m_1d[1] * state.m_1d[5] * state.m_1d[6]
		              + state.m_1d[2] * state.m_1d[3] * state.m_1d[7]
		              - state.m_1d[0] * state.m_1d[5] * state.m_1d[7]
		              - state.m_1d[1] * state.m_1d[3] * state.m_1d[8]
		              - state.m_1d[2] * state.m_1d[4] * state.m_1d[6];

		// Запоминаем информацию о конфигуркции
		States[ id ] = det;
		F[ id ] = det;

		// Шаг цикла
		if( !p.hasNext() )
			break;
		p = p.next();
	}

	// Выполняем рассчёт игры
	while( !F.empty() ) {

		// Шаг обратный максимизации
		f.clear();
		for(i = F.begin(); i != F.end(); ++i) {
			state = unpack(i->first);

			for(k = 0; k < NMAX; ++k) {
				if( state.m_1d[ k ] == 0 )
					continue;

				std::swap(state.m_1d[ k ], z);
				const size_t id = pack(invariant(state));
				std::swap(state.m_1d[ k ], z);

				int det = i->second;

				if( (j = f.find(id)) != f.end() )
					det = std::max(det, j->second);

				f[ id ] = det;
				States[ id ] = det;
			}
		}

		// Шаг обратный минимизации
		F.clear();
		for(i = f.begin(); i != f.end(); ++i) {
			state = unpack(i->first);

			for(k = 0; k < NMAX; ++k) {
				if( state.m_1d[ k ] == 0 )
					continue;

				std::swap(state.m_1d[ k ], z);
				const size_t id = pack(invariant(state));
				std::swap(state.m_1d[ k ], z);

				int det = i->second;
				if( (j = F.find(id)) != F.end() )
					det = std::min(det, j->second);

				F[ id ] = det;
				States[ id ] = det;
			}
		}
	}
}


void writeStates() {
	container_t::const_iterator i;
	size_t num;

	printf("#ifndef __states_h__\n");
	printf("#define __states_h__\n\n");
	printf("#include <cstddef>\n\n\n");

	printf("const size_t states_number = %u;\n\n", States.size());

	printf("const size_t states[] = {");
	for(i = States.begin(), num = 0; i != States.end(); ++i, ++num) {
		if( num % 8 == 0 )
			printf("\n\t");

		printf("0x%08x, ", i->first);
	};
	printf("};\n\n");

	printf("const short costs[] = {");
	for(i = States.begin(), num = 0; i != States.end(); ++i, ++num) {
		if( num % 8 == 0 )
			printf("\n\t");

		printf("0x%04hx, ", i->second);
	};
	printf("};\n\n");

	printf("#endif //_states_h__\n");
}


int main() {
	assert( PMAX >= NMAX );
	assert( NMAX == nmax * nmax );

	freopen("states.h", "w", stdout);

	genStates();
	writeStates();

	return 0;
}
