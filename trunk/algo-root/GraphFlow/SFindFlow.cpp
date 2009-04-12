#include <cstdio>
#include <cstddef>
#include <cassert>
#include <ctime>

#include <algorithm>
#include <queue>

using std::min;
using std::queue;

const long double eps = 1e-10;

const size_t NMAX = 128;

size_t N;
size_t Neig_count[ NMAX ];		// Колличество соседей данной вершины
size_t Neig_cur[ NMAX ];		// Номер текущего соседа в списке
size_t Neig[ NMAX ][ NMAX ];		// Список соседей данной вершины

size_t S, T;				// Номер вершин стока и истока
long double C[ NMAX ][ NMAX ];		// Пропускная способность (capacity)
long double F[ NMAX ][ NMAX ];		// Поток (flow)

size_t H[ NMAX ];			// Высотная функция (hieght function)
long double E[ NMAX ];			// Избыток (excess flow)
bool U[ NMAX ];				// Флаг использования вершины
//queue<size_t> Q;			// Очередь вершин для обработки
size_t St[ NMAX ], t;

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
	
	if( !U[v] ) {
		U[v] = true;
		St[++t] = v;
	}
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

void InitializePreflow() {
	for(size_t u = 0; u < N; ++u) {
		H[u] = 0;
		E[u] = 0;
	}

	for(size_t u = 0; u < N; ++u) {
		for(size_t v = 0; v < N; ++v)
			F[u][v] = F[v][u] = 0;
	}

	H[S] = N;
	for(size_t i = 0; i < Neig_count[S]; ++i) {
		const size_t v = Neig[S][i];
		F[S][v] = C[S][v];
		F[v][S] = - C[S][v];
		E[v] = C[S][v];

		U[v] = true;
		St[++t] = v;		
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

void FindFlow() {
	t = (size_t)-1;
	InitializePreflow();

	while( (size_t)-1 != t ) {
		size_t u = St[t--];
		if( S != u && T != u )
			Discharge(u);
		U[u] = false;
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

void PrintNode(const size_t u) {
	if( S == u )
		printf("S");
	else if( T == u )
		printf("T");
	else
		printf("%u", u);
}

void PrintEdge(const size_t u, const size_t v) {
	PrintNode(u);
	printf(" -> ");
	PrintNode(v);
	printf("\t");
	printf("[%.2Lf]\n", F[u][v]);
}
void PrintFlow() {
	for(size_t i = 0; i < Neig_count[S]; ++i) {
		const size_t v = Neig[S][i];
		if( F[S][v] < eps ) continue;
		PrintEdge(S, v);
	}

	for(size_t u = 0; u < N; ++u) {
		if( S == u || T == u ) continue;

		for(size_t i = 0; i < Neig_count[u]; ++i) {
			const size_t v = Neig[u][i];
			if( F[u][v] < eps ) continue;
			PrintEdge(u, v);
		}
	}
}

int main() {
	N = 6;
	S = 0;
	T = 5;

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
	FindFlow();
	const clock_t stop = clock();

	printf("Flow %.2Lf\n", E[T]);
	PrintFlow();

	printf("Push: %u\nLift: %u\nDischrge: %u\n", push, lift, discharge);

	printf("\ntime %.2Lf\n", (long double)(stop - start) / CLOCKS_PER_SEC);

	return 0;
}