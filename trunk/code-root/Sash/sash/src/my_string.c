#include "my_string.h"
#include "my_errno.h"

size_t my_strlen(const char *s) {
	size_t i;
	for(i = 0; '\0' != s[i]; ++i);
	return i;
}

void my_strcpy(char *dest, const char *src) {
	size_t i;
	for(i = 0; '\0' != (dest[i]=src[i]); ++i);
}

void my_strncpy(char *dest, const char *src, size_t n) {
	size_t i;
	if( 0 == n )
		return;
	for(i = 0; i < n && '\0' != (dest[i]=src[i]); ++i);
	dest[i] = '\0';
}

char *my_strcat(char *dest, const char *src) {
	size_t i = my_strlen(dest), j;
	for(j = 0; '\0' != (dest[i]=src[j]); ++i, ++j);
	return dest;
}

char *my_strncat(char *dest, const char *src, size_t n) {
	size_t i = my_strlen(dest), j;
	for(j = 0; '\0' != (dest[i]=src[j]) && j < n; ++i, ++j);
	return dest;
}

int my_strcmp(const char *s1, const char *s2) {
	size_t i;
	for(i = 0; s1[i] == s2[i] && '\0' != s1[i]; ++i);

	if( s1[i] == s2[i] )
		return 0;
	else if( s1[i] > s2[i] )
		return 1;
	else
		return -1;
}

int my_strncmp(const char *s1, const char *s2, size_t n) {
	size_t i;
	
	for(i = 0; s1[i] == s2[i] && '\0' != s1[i] && i < n; ++i);

	if( i == n || s1[i] == s2[i] )
		return 0;
	else if( s1[i] > s2[i] )
		return 1;
	else
		return -1;
}

char * my_l2str(char *dest, long s) {
	char buf[16] = {'\0'};
	char *c = buf + 1;
	char *d = dest;

	if( s < 0 ) {
		*d = '-';
		++d;
		s = -s;
	}
	do {
		*c = '0' + s%10;
		s /= 10;
		++c;
	} while( s );

	do {
		--c;
		*d = *c;
		++d;
	} while( c != buf );

	return dest;
}

bool my_str2l(const char *src, long *l) {
	// В этой процедуре заложена потенциальная опасность!
	
	unsigned char *c = (unsigned char *)src;

	long u = 0, u_old = 0;
	
	int sign = 1;

	bool ok = false;
	
	while( ' ' == *c || '\t' == *c )
		++c;
	
	if( '-' == *c ) {
		sign = -1;
		++c;
	} else if( '+' == *c ) {
		sign = 1;
		++c;
	}
	
	while( '0' <= *c && '9' >= *c && u >= u_old ) {
		ok = true;
		u_old = u;
		u = u*10 + *c - '0';
		++c;
	}
	
	if( !ok ) {
		errno = EINVAL;
		return false;
	}
	
	if( u >= u_old && (1 != sign || u < 0x80000000) ) {
		*l = u*sign;
		return true;
	}
	
	errno = ERANGE;
	return false;
	
}
