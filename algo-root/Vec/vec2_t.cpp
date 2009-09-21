class vec_t {
public:
  long double x, y;

  vec_t() {}
  vec_t(long double _x, long double _y) : x(_x), y(_y) {}
  vec_t(const vec_t & _v) : x(_v.x), y(_v.y) {}

  vec_t & operator = (const vec_t & _v) {
    x = _v.x;
    y = _v.y;
    return * this;
  }

  vec_t & operator += (const vec_t & _v) {
    x += _v.x;
    y += _v.y;
    return * this;
  }

  vec_t & operator -= (const vec_t & _v) {
    x -= _v.x;
    y -= _v.y;
    return * this;
  }

  vec_t & operator *= (long double _c) {
    x *= _c;
    y *= _c;
    return * this;
  }

  vec_t & operator /= (long double _c) {
    x /= _c;
    y /= _c;
    return * this;
  }

  long double norm() const {
    return x*x + y*y;
  }

  long double length() const {
    return sqrtl(norm());
  }
};

const vec_t operator + (const vec_t & _u) {
  return _u;
}

const vec_t operator - (const vec_t & _u) {
  return vec_t(-_u.x, -_u.y);
}

const vec_t operator + (const vec_t & _u, const vec_t & _v) {
  return vec_t(_u) += _v;
}
const vec_t operator - (const vec_t & _u, const vec_t & _v) {
  return vec_t(_u) -= _v;
}
const vec_t operator * (const vec_t & _u, long double _c) {
  return vec_t(_u) *= _c;
}
const vec_t operator * (long double _c, const vec_t & _v) {
  return vec_t(_v) *= _c;
}
const vec_t operator / (const vec_t & _u, long double _c) {
  return vec_t(_u) /= _c;
}

long double smul(const vec_t & _u, const vec_t & _v) {
  return _u.x*_u.x + _u.y*_u.y;
}
long double vmul(const vec_t & _u, const vec_t & _v) {
  return _u.x*_v.y - _u.y*_v.x;
}

const lrot(const vec_t & _u) {
  return vec_t(-_u.y, _u.x);
}

bool intersect(vec_t & r, const vec_t & u1, const vec_t & u2, const vec_t & v1, const vec_t & v2) {
  const long double m11 = u1.x - u2.x, m12 = u1.y - u2.y;
  const long double m21 = v2.x - v1.x, m22 = v2.y - v1.y;

  const long double det = m11*m22 - m12*m21;

  if( fabsl(det) < eps )
    return false;

  const long double r11 =  m22, r12 = -m12;
  const long double r21 = -m21, r22 =  m11;

  const long double a = u1.x - v1.x, b = u1.y - v1.y;

  const long double t = (a*r11 + b*r21)/det, s = (a*r12 + b*r22)/det;

  r = u1 + (u2 - u1)*t;

  return true;
}
