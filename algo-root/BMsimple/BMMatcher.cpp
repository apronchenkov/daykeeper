#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>

class BMsimple_t {
	size_t * bc;
	size_t * gs;
	char * str;
	size_t len;

	char Str(size_t i) { return str[i]; }
	char rStr(size_t i) { return str[len - 1 - i]; }

	void reset_bc() {
		memset(bc, 0, 256*sizeof(bc[0]));
		for(size_t i = 0; i < len; ++i)
			bc[(unsigned char)str[i]] = i;
	}

	void reset_pi(size_t * pi) {
		pi[0] = pi[1] = 0;
		size_t i = 1, j = 0;
		while( Str(i) ) {
			for(; j && Str(j) != Str(i); j = pi[j]);
			if( Str(j) == Str(i) )
				++j;
			pi[++i] = j;
		}
	}


	void reset_rpi(size_t * rpi) {
		rpi[0] = rpi[1] = 0;
		size_t i = 1, j = 0;
		while( rStr(i) ) {
			for(; j && rStr(j) != rStr(i); j = rpi[j]);
			if( rStr(j) == rStr(i) )
				++j;
			rpi[++i] = j;
		}
	}

	void reset_gs() {
		size_t * pi = new size_t[len + 1];
		reset_pi(pi);
		for(size_t i = 0; i < len + 1; ++i)
			gs[i] = len - pi[len];
		
		size_t * rpi = pi;
		reset_rpi(rpi);
		for(size_t l = 1, i = len - rpi[0]; l < len + 1; ++l, i = len - rpi[l]) 
			if( gs[i] > l - rpi[l] )
				gs[i] = l - rpi[l];

		delete [] pi;
	}
public:
	BMsimple_t(const char * _str) : len(strlen(_str)), bc(NULL), gs(NULL), str(NULL) {
		assert( len );
		try {
			bc = new size_t[256];
			gs = new size_t[len + 1];
			str = new char[len + 1];

			memcpy(str, _str, len + 1);
			reset_bc();
			reset_gs();

		} catch(...) {
			delete [] bc;
			delete [] gs;
			delete [] str;
			throw;
		}
	}
	
	~BMsimple_t() {
		delete [] bc;
		delete [] gs;
		delete [] str;
	}

	size_t search(const char * _str) const {
		size_t Len = strlen(_str);
		for(size_t s = 0, j; s + len <= Len;) {
			for(j = len - 1; j && str[j] == _str[s + j]; --j);
			if( str[j] == _str[s + j] )
				return s;
			if( j < gs[j + 1] + bc[_str[s + j]] )
				s += gs[j + 1];
			else
				s += j - bc[_str[s + j]];
		}

		return -1;
	}
};

int main() {
	const BMsimple_t bm("ANPANMAN");
	const char * str = "hello, World! hello, hehello";
	
	size_t i, j;
	for(i = j = bm.search(str); (size_t)-1 != j; j = bm.search(str + i + 1), i += j + 1) 
		printf("%u: %s\n", i, str + i);

	return 0;
}