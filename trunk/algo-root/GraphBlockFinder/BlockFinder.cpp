#include <cstdio>
#include <cstring>
#include <cerrno>

#include <tchar.h>

#include <list>
#include <stack>
#include <algorithm>
#include <utility>

#include <locale.h>

using std::list;
using std::stack;
using std::min;
using std::pair;
using std::make_pair;

const size_t NMAX = 10240;

void usage() {
	_ftprintf(stderr, _T("usage: file[1] ... file[n]\n"));
}

typedef list<size_t> edges_t;	/* тип для списка соседей данной вершины */
typedef edges_t::const_iterator edges_citer_t;

struct graph_t {
	
	size_t n;				/* количество вершин в графе */
	
	edges_t * edges;			/* массив списков соседей для всех вершин */

	graph_t() : n(0), edges(NULL) { 
	}

	~graph_t() {
		delete [] edges;
	}

	bool scan(FILE * in) {
		_TCHAR tmp[32];

		if( 1 != _ftscanf(in, _T("%u"), &n) ) {
			if( ferror(in) )
				goto _io_error;
			_ftprintf(stderr, _T("Ошибка: не удалось считать число вершин графа\n"));
			return false;
		}

		if( n > NMAX ) {
			_ftprintf(stderr, _T("Ошибка: объявлено слишком большое число вершин\n"));
			return false;
		}

		if( 1 != _ftscanf(in, _T("%31s"), tmp) ) {
			if( ferror(in) )
				goto _io_error;
			_ftprintf(stderr, _T("Ошибка: не удалось считать вспомогательно поле Размер Вершины\n"));
			return false;
		}

		edges = new edges_t[n];
		
		size_t u, v;
		while( 3 == _ftscanf(in, _T("%u%1s%u"), &u, tmp, &v) ) {
			if( n <= u || n <= v ) {
				_ftprintf(stderr, _T("Ошибка: ребро (%u,%u) связывает необъявленные вершины\n"), u, v);
				return false;
			}

			edges[u].push_back(v);
			edges[v].push_back(u);
		}

		if( ferror(in) )
			goto _io_error;

		return true;

_io_error:
		_ftprintf(stderr, _T("fscanf: %s\n"), _tcserror(errno));
		return false;
	}
};

void BiComp(const graph_t & G, const size_t root, size_t * num, size_t & i, size_t * L, size_t * father, edges_citer_t * citer) {
	stack<size_t> S;
	stack<size_t> SB;

	father[root] = (size_t)-1;
	num[root] = L[root] = i++;

	if( 0 == G.edges[root].size() ) {
		_tprintf(_T("%u\n"), root);
		return;
	}

	S.push(root);
	//SB.push(root);

	while( !S.empty() ) {
		const size_t v = S.top();
		edges_citer_t u = citer[v];

		if( G.edges[v].end() == u ) {
			S.pop();
			continue;
		}

		if( v == father[*u] ) {
			L[v] = min(L[v], L[*u]);
			
			if( num[v] <= L[*u] ) {
				while( SB.top() != *u ) {
					_tprintf(_T("%u "), SB.top());
					SB.pop();
				}
				_tprintf(_T("%u %u\n"), *u, v);
				SB.pop();
			}
			++u;
		}

		while( G.edges[v].end() != u && 0 != num[*u] ) {
			if( num[*u] < num[v] && *u != father[v] )
				L[v] = min(L[v], num[*u]);
			++u;
		}

		if( G.edges[v].end() != u ) {
			S.push(*u);
			SB.push(*u);
			father[*u] = v;
			num[*u] = L[*u] = i++;
		}

		citer[v] = u;
	}
}

void block_founder(const graph_t & G) {
	size_t num[NMAX], i = 1;
	memset(num, 0, G.n * sizeof(num[0]));

	size_t L[NMAX];
	size_t father[NMAX];

	edges_citer_t citer[NMAX];
	for(size_t j = 0; j < G.n; ++j)
		citer[j] = G.edges[j].begin();

	_tprintf(_T("Список блоков:\n"));

	for(size_t v = 0; v < G.n; ++v) {
		if( !num[v] )
			BiComp(G, v, num, i, L, father, citer);
	}
	
	_tprintf(_T("\n"));

	_tprintf(_T("Список точек сочленения:\n"));

	size_t deg[NMAX];
	memset(deg, 0, G.n * sizeof(deg[0]));

	for(size_t j = 0; j < G.n; ++j) {	// Вычисляем степень вершин в лесе поиска
		size_t f = father[j];
		if( (size_t)-1 != f )
			++deg[f];
	}
			
	for(size_t j = 0; j < G.n; ++j) {	// Выполняем проверку 
		const size_t f = father[j];
		if( (size_t)-1 != deg[f] && (size_t)-1 != f ) {
			if( (size_t)-1 == father[f] ) {
				if( 1 < deg[f] ) {
					_tprintf(_T("%u "), f);
					deg[f] = (size_t)-1;
				}
			} else {
				if( num[f] <= L[j] ) {
					_tprintf(_T("%u "), f);
					deg[f] = (size_t)-1;
				}
			}
		}
	}

	_tprintf(_T("\n\n"));
}

int _tmain(size_t argc, _TCHAR **argv) {
	setlocale( LC_ALL, "Russian" );

	if( 1 == argc ) {
		usage();
		return 0;
	}


	for(size_t i = 1; i < argc; ++i) {
		FILE * in;
		graph_t G;
	
		_tprintf(_T("Граф %s:\n"), argv[i]);

		if( NULL == (in = _tfopen(argv[i], _T("rt"))) ) {
			_ftprintf(stderr, _T("fopen: %s: %s\n"), argv[i], _tcserror(errno));
			continue;
		}

		if( !G.scan(in) ) {
			fclose(in);
			continue;
		}
		
		fclose(in);

		block_founder(G);
	}


	return 0;
}
