class vec_t {
public:
  long double x, y, z;

  vec_t() {}
  vec_t(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}
  vec_t(const vec_t & _v) : x(_v.x), y(_v.y), z(_v.z) {}

  vec_t & operator = (const vec_t & _v) {
    x = _v.x;
    y = _v.y;
    z = _v.z;
    return * this;
  }

  vec_t & operator += (const vec_t & _v) {
    x += _v.x;
    y += _v.y;
    z += _v.z;
    return * this;
  }

  vec_t & operator -= (const vec_t & _v) {
    x -= _v.x;
    y -= _v.y;
    z -= _v.z;
    return * this;
  }

  vec_t & operator *= (long double _c) {
    x *= _c;
    y *= _c;
    z *= _c;
    return * this;
  }

  vec_t & operator /= (long double _c) {
    x /= _c;
    y /= _c;
    z /= _c;
    return * this;
  }

  long double norm() const {
    return x*x + y*y + z*z;
  }

  long double length() const {
    return sqrtl(x*x + y*y + z*z);
  }
};

const vec_t operator + (const vec_t & _u) {
  return _u;
}
const vec_t operator - (const vec_t & _u) {
  return vec_t(-_u.x, -_u.y, -_u.z);
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
  return _u.x*_u.x + _u.y*_u.y + _u.z*_u.z;
}
const vec_ vmul(const vec_t & _u, const vec_t & _v) {
  return vec_t(_u.y*_v.z - _u.z*_v.y, - _u.x*_v.z + _u.z*_v.x, _u.x*_v.y - _u.y*_v.x);
}
