#include <cstdio>
#include <cstddef>
#include <cassert>
#include <ctime>

#include <algorithm>
#include <list>

using std::min;
using std::list;

const long double eps = 1e-10;

const size_t NMAX = 128;

size_t N;
size_t Neig_count[ NMAX ];		// Колличество соседей данной вершины
size_t Neig_cur[ NMAX ];		// Номер текущего соседа в списке
size_t Neig[ NMAX ][ NMAX ];		// Список соседей данной вершины

long double C[ NMAX ][ NMAX ];		// Пропускная способность (capacity)
long double F[ NMAX ][ NMAX ];		// Поток (flow)

size_t H[ NMAX ];			// Высотная функция (hieght function)
long double E[ NMAX ];			// Избыток (excess flow)
size_t push, lift, discharge;

long double Cf(size_t u, size_t v) {	// Остаточная пропускная способность (residual capacity)
	return C[u][v] - F[u][v];
}

void Push(size_t u, size_t v) {
	++push;

	assert( H[u] == H[v] + 1 );
	assert( Cf(u, v) > eps );

	const long double df = min(E[u], Cf(u, v));
	F[u][v] += df;
	F[v][u] = - F[u][v];
	E[u] -= df;
	E[v] += df;
}

void Lift(size_t u) {
	++lift;

	assert( E[u] > eps );

	size_t h = (size_t)-1;
	for(size_t i = 0; i < Neig_count[u]; ++i) {
		const size_t v = Neig[u][i];
		if( Cf(u, v) )
			h = min(h, H[v]);
	}
	
	assert( H[u] < h + 1 );

	H[u] = h + 1;
}

void InitializePreflow(size_t s) {
	for(size_t u = 0; u < N; ++u) {
		H[u] = 0;
		E[u] = 0;
	}

	for(size_t u = 0; u < N; ++u) {
		for(size_t v = 0; v < N; ++v)
			F[u][v] = F[v][u] = 0;
	}

	H[s] = N;
	for(size_t i = 0; i < Neig_count[s]; ++i) {
		const size_t v = Neig[s][i];
		F[s][v] = C[s][v];
		F[v][s] = - C[s][v];
		E[v] = C[s][v];
	}
}

void Discharge(size_t u) {
	++discharge;

	while( E[u] > eps ) {
		if( Neig_cur[u] == Neig_count[u] ) {
			Lift(u);
			Neig_cur[u] = 0;
		}
		const size_t v = Neig[u][ Neig_cur[u] ];
		if( H[u] == H[v] + 1 && Cf(u, v) > eps )
			Push(u, v);
		else
			++Neig_cur[u];
	}
}

void LiftToFront(const size_t s, const size_t t) {
	InitializePreflow(s);

	list<size_t> L;
	list<size_t>::iterator _u;

	for(size_t u = 0; u < N; ++u) {
		if( s == u || t == u ) continue;
		L.push_back(u);
	}

	for(_u = L.begin(); L.end() != _u; ++_u) {
		const size_t u = *_u;
		const size_t old_H = H[u];
		Discharge(u);
		
		if( H[u] > old_H ) {
			L.erase(_u);
			L.push_front(u);
			_u = L.begin();
		}
	}
}

void CalculateNeig() {
	for(size_t u = 0; u < N; ++u) {
		for(size_t v = 0; v < N; ++v) {
			if( C[u][v] < eps && C[v][u] < eps ) continue;
			Neig[u][ Neig_count[u]++ ] = v;
		}
	}
}

void PrintNode(const size_t u, const size_t s, const size_t t) {
	if( s == u )
		printf("S");
	else if( t == u )
		printf("T");
	else
		printf("%u", u);
}

void PrintEdge(const size_t u, const size_t v, const size_t s, const size_t t) {
	PrintNode(u, s, t);
	printf(" -> ");
	PrintNode(v, s, t);
	printf("\t");
	printf("[%.2Lf]\n", F[u][v]);
}
void PrintFlow(const size_t s, const size_t t) {
	for(size_t i = 0; i < Neig_count[s]; ++i) {
		const size_t v = Neig[s][i];
		if( F[s][v] < eps ) continue;
		PrintEdge(s, v, s, t);
	}

	for(size_t u = 0; u < N; ++u) {
		if( s == u || t == u ) continue;

		for(size_t i = 0; i < Neig_count[u]; ++i) {
			const size_t v = Neig[u][i];
			if( F[u][v] < eps ) continue;
			PrintEdge(u, v, s, t);
		}
	}
}

int main() {
	N = 6;

	const size_t S = 0, T = 5;

	C[0][1] = 16;
	C[0][2] = 13;
	C[1][2] = 10;
	C[1][3] = 12;
	C[2][1] = 4;
	C[2][4] = 14;
	C[3][2] = 9;
	C[3][5] = 20;
	C[4][3] = 7;
	C[4][5] = 4;

	CalculateNeig();

	const clock_t start = clock();
	LiftToFront(S, T);
	const clock_t stop = clock();

	printf("Flow %.2Lf\n", E[T]);
	PrintFlow(S, T);
	printf("Push: %u\nLift: %u\nDischrge: %u\n", push, lift, discharge);

	printf("\ntime %.2Lf\n", (long double)(stop - start) / CLOCKS_PER_SEC);

	return 0;
}