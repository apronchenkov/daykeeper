#ifndef __GFn_h__
#define __GFn_h__ 1

#include "GFpoly.h"
#include <iostream>

class GFn_t {

	GFn_t(const GFn_t &);
public:

	GFn_t(unsigned int _prime, int _degree, const unsigned int *_mod);
	~GFn_t();

	unsigned int prime;
	int degree;

	unsigned int *mod;
};

class GFn_el_t {

public:

	GFn_el_t(const GFn_t &_GFn);	
	GFn_el_t(const GFn_t &_GFn, int _a);
	GFn_el_t(const GFn_t &_GFn, unsigned int _a);
	GFn_el_t(const GFn_t &_GFn, unsigned int _a[]);
	GFn_el_t(const GFn_el_t &_h);

	~GFn_el_t();

	GFn_el_t & operator = (const GFn_el_t &h);
	GFn_el_t & operator = (int _a);
	GFn_el_t & operator = (unsigned int _a);

	bool operator == (const GFn_el_t &_h) const;
	bool operator == (int _a) const;
	bool operator == (unsigned int _a) const;

	bool operator != (const GFn_el_t &_h) const;
	bool operator != (int _a) const;
	bool operator != (unsigned int _a) const;
	
	GFn_el_t & operator += (const GFn_el_t &_h);
	GFn_el_t & operator += (int _a);
	GFn_el_t & operator += (unsigned int _a);

	GFn_el_t & operator -= (const GFn_el_t &_h);
	GFn_el_t & operator -= (int _a);
	GFn_el_t & operator -= (unsigned int _a);

	GFn_el_t & operator *= (const GFn_el_t &_h);
	GFn_el_t & operator *= (int _a);
	GFn_el_t & operator *= (unsigned int _a);

	GFn_el_t & operator /= (const GFn_el_t &_h);
	GFn_el_t & operator /= (int _a);
	GFn_el_t & operator /= (unsigned int _a);

	const GFn_el_t operator + () const;

	const GFn_el_t operator - () const;

	const GFn_el_t inv() const;

	unsigned int & operator [] (int _i);
	unsigned int operator [] (int _i) const;

	unsigned int *comp;
	
	const GFn_t &GFn;
	
	const unsigned int prime;
	const int degree;
};

bool operator == (int _a, const GFn_el_t &_h);
bool operator != (int _a, const GFn_el_t &_h);
bool operator == (unsigned int _a, const GFn_el_t &_h);
bool operator != (unsigned int _a, const GFn_el_t &_h);

const GFn_el_t operator + (const GFn_el_t &_g, const GFn_el_t &_h);
const GFn_el_t operator + (const GFn_el_t &_g, int _a);
const GFn_el_t operator + (const GFn_el_t &_g, unsigned int _a);
const GFn_el_t operator + (int a, const GFn_el_t &_h);
const GFn_el_t operator + (unsigned int _a, const GFn_el_t &_h);

const GFn_el_t operator - (const GFn_el_t &_g, const GFn_el_t &_h);
const GFn_el_t operator - (const GFn_el_t &_g, int _a);
const GFn_el_t operator - (const GFn_el_t &_g, unsigned int _a);
const GFn_el_t operator - (int a, const GFn_el_t &_h);
const GFn_el_t operator - (unsigned int _a, const GFn_el_t &_h);

const GFn_el_t operator * (const GFn_el_t &_g, const GFn_el_t &_h);
const GFn_el_t operator * (const GFn_el_t &_g, int _a);
const GFn_el_t operator * (const GFn_el_t &_g, unsigned int _a);
const GFn_el_t operator * (int a, const GFn_el_t &_h);
const GFn_el_t operator * (unsigned int _a, const GFn_el_t &_h);

const GFn_el_t operator / (const GFn_el_t &_g, const GFn_el_t &_h);
const GFn_el_t operator / (const GFn_el_t &_g, int _a);
const GFn_el_t operator / (const GFn_el_t &_g, unsigned int _a);
const GFn_el_t operator / (int a, const GFn_el_t &_h);
const GFn_el_t operator / (unsigned int _a, const GFn_el_t &_h);

std::ostream & operator << (std::ostream &_out, const GFn_el_t &_g);
std::istream & operator >> (std::istream &_in, const GFn_el_t &_g);

#endif // __GFn_h__
