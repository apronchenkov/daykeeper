#ifndef __type_h__
#define __type_h__

typedef signed char		i8_t;
typedef unsigned char		u8_t;
typedef signed short		i16_t;
typedef unsigned short		u16_t;
typedef signed int		i32_t;
typedef unsigned int		u32_t;
typedef signed long long	i64_t;
typedef unsigned long long	u64_t;

typedef u16_t c_t;

template<class T>void swap(T&a, T&b) {T c = a; a = b; b = c;}
template<class T>const T min2(const T &a, const T &b) {return (a <= b) ? a : b;}
template<class T>const T max2(const T &a, const T &b) {return (a >= b) ? a : b;}

#endif // __type_h__
