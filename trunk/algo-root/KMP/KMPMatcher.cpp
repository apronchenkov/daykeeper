#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>

class KMP_t {
	size_t * pi;
	char * str;
	size_t len;

	void reset_pi() {
		pi[0] = pi[1] = 0;
		size_t i = 1, j = 0;
		while( str[i] ) {
			for(; j && str[j] != str[i]; j = pi[j]);
			if( str[j] == str[i] )
				++j;
			pi[++i] = j;
		}
	}
public:
	KMP_t(const char * _str) : len(strlen(_str)), pi(NULL), str(NULL) {
		assert( len );
		try {
			pi = new size_t[len + 1];
			str = new char[len + 1];
		} catch(...) {
			delete [] pi;
			throw;
		}
		memcpy(str, _str, len + 1);
		reset_pi();
	}
	
	~KMP_t() {
		delete [] pi;
		delete [] str;
	}

	size_t search(const char * _str) const {
		size_t i = 0, j = 0;
		while( _str[i] ) {
			for(; j && str[j] != _str[i]; j = pi[j]);
			if( str[j] == _str[i] ) {
				++j;
				if( len == j )
					return i - len + 1;
			}
			++i;
		}
		return -1;
	}
};

int main() {
	const KMP_t kmp("hello");
	const char * str = "hello, World! hello, hehello";

	for(size_t i = 0, j = kmp.search(str); (size_t)-1 != j; j = kmp.search(str + i + 1), i += j + 1) 
		printf("%u: %s\n", i, str + i);

	return 0;
}
