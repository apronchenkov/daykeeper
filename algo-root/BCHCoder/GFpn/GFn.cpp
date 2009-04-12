#include "GFn.h"
#include "GFpoly.h"

#ifndef NULL
#define NULL 0
#endif // NULL

GFn_t::GFn_t(const unsigned int _prime, int _degree, const unsigned int *_mod) : prime(_prime), degree(_degree) {
	int i;

	mod = new unsigned int [degree + 1];

	for(i = 0; i <= degree; ++i)
		mod[i] = _mod[i];
}

GFn_t::~GFn_t() {
	delete mod;
}

GFn_el_t::GFn_el_t(const GFn_t &_GFn): GFn(_GFn), prime(_GFn.prime), degree(_GFn.degree - 1) {

	comp = new unsigned int [degree + 1];
}
GFn_el_t::GFn_el_t(const GFn_t &_GFn, int _a): GFn(_GFn), prime(_GFn.prime), degree(_GFn.degree - 1) {
	int i;

	comp = new unsigned int [degree + 1];

	for(i = degree; i > 0; --i)
		comp[i] = 0;

	comp[0] = gf_int(prime, _a);
}
GFn_el_t::GFn_el_t(const GFn_t &_GFn, unsigned int _a): GFn(_GFn), prime(_GFn.prime), degree(_GFn.degree - 1) {
	int i;

	comp = new unsigned int [degree + 1];

	for(i = degree; i > 0; --i)
		comp[i] = 0;

	comp[0] = _a%prime;
}
GFn_el_t::GFn_el_t(const GFn_t &_GFn, unsigned int _a[]): GFn(_GFn), prime(_GFn.prime), degree(_GFn.degree - 1) {
	int i;

	comp = new unsigned int [degree + 1];

	for(i = degree; i >= 0; --i)
		comp[i] = _a[i];
}
GFn_el_t::GFn_el_t(const GFn_el_t &_h): GFn(_h.GFn), prime(_h.prime), degree(_h.degree) {

	comp = new unsigned int [degree + 1];

	for(int i = degree; i >= 0; --i)
		comp[i] = _h.comp[i];
}

GFn_el_t::~GFn_el_t() {
	delete comp;
}

GFn_el_t & GFn_el_t::operator = (const GFn_el_t &_h) {

	assert( &GFn == &_h.GFn );

	for(int i = degree; i >= 0; --i)
		comp[i] = _h.comp[i];

	return *this;
}
GFn_el_t & GFn_el_t::operator = (int _a) {

	for(int i = degree; i > 0; --i)
		comp[i] = 0;

	comp[0] = gf_int(prime, _a);

	return *this;
}
GFn_el_t & GFn_el_t::operator = (unsigned int _a) {
	
	for(int i = degree; i > 0; --i)
		comp[i] = 0;
	
	comp[0] = _a%prime;

	return *this;
}

bool GFn_el_t::operator == (const GFn_el_t &_h) const {

	assert( &GFn == &_h.GFn );

	for(int i = degree; i >= 0; --i)
		if( comp[i] != _h.comp[i] )
			return false;

	return true;
}
bool GFn_el_t::operator == (int _a) const {
	
	for(int i = degree; i > 0; --i)
		if( 0 != comp[i] )
			return false;

	return gf_int(prime, _a) == comp[0];
}
bool GFn_el_t::operator == (unsigned int _a) const {
	
	for(int i = degree; i > 0; --i)
		if( 0 != comp[i] ) 
			return 0;
			
	return _a%prime == comp[0];
}

bool GFn_el_t::operator != (const GFn_el_t &_h) const {
	return !(*this == _h);
}
bool GFn_el_t::operator != (int _a) const {
	return !(*this == _a);
}
bool GFn_el_t::operator != (unsigned int _a) const {
	return !(*this == _a);
}

GFn_el_t & GFn_el_t::operator += (const GFn_el_t &_h) {

	assert( &GFn == &_h.GFn );

	poly_add(prime, degree, comp, degree, comp, degree, _h.comp);

	return *this;
}
GFn_el_t & GFn_el_t::operator += (int _a) {

	comp[0] = gf_add(prime, comp[0], gf_int(prime, _a));

	return *this;
}
GFn_el_t & GFn_el_t::operator += (unsigned int _a) {

	comp[0] = gf_add(prime, comp[0], _a%prime);

	return *this;
}

GFn_el_t & GFn_el_t::operator -= (const GFn_el_t &_h) {

	assert( &GFn == &_h.GFn );

	poly_sub(prime, degree, comp, degree, comp, degree, _h.comp);

	return *this;
}
GFn_el_t & GFn_el_t::operator -= (int _a) {

	comp[0] = gf_sub(prime, comp[0], gf_int(prime, _a));

	return *this;
}
GFn_el_t & GFn_el_t::operator -= (unsigned int _a) {
	comp[0] = gf_sub(prime, comp[0], _a%prime);

	return *this;
}

GFn_el_t & GFn_el_t::operator *= (const GFn_el_t &_h) {

	assert( &GFn == &_h.GFn );

	unsigned int M[2*degree + 1];
	
	poly_mul(prime, 2*degree, M, degree, comp, degree, _h.comp);

	poly_divmod(prime, -1, NULL, degree, comp, 2*degree, M, GFn.degree, GFn.mod);

	return *this;
}
GFn_el_t & GFn_el_t::operator *= (int _a) {
	unsigned int a = gf_int(prime, _a);

	poly_mul(prime, degree, comp, degree, comp, 0, &a);

	return *this;
}
GFn_el_t & GFn_el_t::operator *= (unsigned int _a) {	
	unsigned int a = _a%prime;

	poly_mul(prime, degree, comp, degree, comp, 0, &a);

	return *this;
}

GFn_el_t & GFn_el_t::operator /= (const GFn_el_t &_h) {
	return *this *= _h.inv();
}
GFn_el_t & GFn_el_t::operator /= (int _a) {
	unsigned int a = gf_inv(prime, gf_int(prime, _a));

	poly_mul(prime, degree, comp, degree, comp, 0, &a);

	return *this;
}
GFn_el_t & GFn_el_t::operator /= (unsigned int _a) {
	unsigned int a = gf_inv(prime, _a%prime);

	poly_mul(prime, degree, comp, degree, comp, 0, &a);

	return *this;
}

const GFn_el_t GFn_el_t::operator +() const {
	return GFn_el_t(*this);
}

const GFn_el_t GFn_el_t::operator -() const {
	GFn_el_t T(GFn);

	poly_sub(prime, degree, T.comp, -1, NULL, degree, comp);
	
	return T;
}

const GFn_el_t GFn_el_t::inv() const {	
	int i;

	unsigned int u0[degree + 1], r0[degree + 1];
	unsigned int u1[degree + 1], r1[degree + 1];

	unsigned int q[degree + 1], r[degree + 1];

	unsigned int u[2*degree + 1];

	{
		for(i = degree; i >= 0 && 0 == comp[i]; --i)
			r1[i] = 0;

		assert( i >= 0 );
		
		if( 0 == i )
			return GFn_el_t(GFn, gf_inv(prime, comp[0]));

		for(; i >= 0; --i)
			r1[i] = comp[i];
		
		for(i = degree; i >= 1; --i)
			u1[i] = 0;
		
		u1[0] = 1;
	}
	{
		poly_divmod(prime, degree, u0, degree, r0, GFn.degree, GFn.mod, degree, comp);

		poly_sub(prime, degree, u0, -1, NULL, degree, u0);
	}

	while( true ) {

		{
			for(i = degree; i >= 0 && 0 == r0[i] ; --i);
		
			assert( i >= 0 );

			if( 0 == i ) {
				unsigned int inv = gf_inv(prime, r0[0]);

				poly_mul(prime, degree, u0, degree, u0, 0, &inv);
				
				return GFn_el_t(GFn, u0);
			}
		}
		
		poly_divmod(prime, degree, q, degree, r, degree, r1, degree, r0);
		
		for(i = degree; i >= 0; --i) {
			r1[i] = r0[i];
			r0[i] = r[i];
		}
		
		poly_mul(prime, 2*degree, u, degree, u0, degree, q);
		poly_sub(prime, 2*degree, u, degree, u1, 2*degree, u);
		
		for( i = degree; i >= 0; --i)
			u1[i] = u0[i];
		
		poly_divmod(prime, -1, NULL, degree, u0, 2*degree, u, GFn.degree, GFn.mod);
	}
}

unsigned int & GFn_el_t::operator [] (int _i) {
	return comp[_i];
}

unsigned int GFn_el_t::operator [] (int _i) const {
	return comp[_i];
}

bool operator == (int _a, const GFn_el_t &_h) {
	return _h == _a;
}
bool operator != (int _a, const GFn_el_t &_h) {
	return _h != _a;
}
bool operator == (unsigned int _a, const GFn_el_t &_h) {
	return _h == _a;
}
bool operator != (unsigned int _a, const GFn_el_t &_h) {
	return _h != _a;
}

const GFn_el_t operator + (const GFn_el_t &_g, const GFn_el_t &_h) {
	return GFn_el_t(_g) += _h;
}
const GFn_el_t operator + (const GFn_el_t &_g, int _a) {
	return GFn_el_t(_g) += _a;
}
const GFn_el_t operator + (int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) += _a;
}
const GFn_el_t operator + (const GFn_el_t &_g, unsigned int _a) {
	return GFn_el_t(_g) += _a;
}
const GFn_el_t operator + (unsigned int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) += _a;
}

const GFn_el_t operator - (const GFn_el_t &_g, const GFn_el_t &_h) {
	return GFn_el_t(_g) -= _h;
}
const GFn_el_t operator - (const GFn_el_t &_g, int _a) {
	return GFn_el_t(_g) -= _a;
}
const GFn_el_t operator - (int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) -= _a;
}
const GFn_el_t operator - (const GFn_el_t &_g, unsigned int _a) {
	return GFn_el_t(_g) -= _a;
}
const GFn_el_t operator - (unsigned int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) -= _a;
}

const GFn_el_t operator * (const GFn_el_t &_g, const GFn_el_t &_h) {
	return GFn_el_t(_g) *= _h;
}
const GFn_el_t operator * (const GFn_el_t &_g, int _a) {
	return GFn_el_t(_g) *= _a;
}
const GFn_el_t operator * (int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) *= _a;
}
const GFn_el_t operator * (const GFn_el_t &_g, unsigned int _a) {
	return GFn_el_t(_g) *= _a;
}
const GFn_el_t operator * (unsigned int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) *= _a;
}

const GFn_el_t operator / (const GFn_el_t &_g, const GFn_el_t &_h) {
	return GFn_el_t(_g) /= _h;
}
const GFn_el_t operator / (const GFn_el_t &_g, int _a) {
	return GFn_el_t(_g) /= _a;
}
const GFn_el_t operator / (int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) /= _a;
}
const GFn_el_t operator / (const GFn_el_t &_g, unsigned int _a) {
	return GFn_el_t(_g) /= _a;
}
const GFn_el_t operator / (unsigned int _a, const GFn_el_t &_h) {
	return GFn_el_t(_h) /= _a;
}

std::ostream & operator << (std::ostream &_out, const GFn_el_t &_g) {

	for(int i = 0; i <= _g.degree; ++i)
		_out << _g.comp[i] << "\t";

	return _out;
}

std::istream & operator >> (std::istream &_in, const GFn_el_t &_g) {

	for(int i = 0; i <= _g.degree; ++i)
		_in >> _g.comp[i];

	return _in;
}
