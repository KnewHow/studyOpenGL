#pragma once

#include<cassert>
#include<cmath>
#include<iostream>

namespace math
{
    //------------------------------- define some vector --------------------------------------------

template<class T, int n> struct vec
{
    vec() = default;
    T& operator[](const int i) {
        assert(i>=0 && i < n);
        return data[i];
    }
    T operator[](const int i) const {
        assert(i>=0 && i < n);
        return data[i];
    }
    double norm2() const {
        return (*this) * (*this);
    }
    double norm() const {
        return std::sqrt(norm2());
    }
    T data[n] = {};
};

template<class T, int n> T operator*(const vec<T, n>& lhs, const vec<T, n>& rhs) {
    T ret = 0;
    for(int i = n; i--; ret += lhs[i]*rhs[i]);
    return ret;
}


template<class T, int n> vec<T, n> operator+(const vec<T, n>& lhs, const vec<T, n>& rhs){
    vec<T, n> ret = lhs;
    for(int i = n; i--; ret[i] += rhs[i]);
    return ret;
}

template<class T, int n> vec<T, n> operator-(const vec<T, n>& lhs, const vec<T, n>& rhs){
    vec<T, n> ret = lhs;
    for(int i = n; i--; ret[i] -= rhs[i]);
    return ret;
}

template<class T, int n> vec<T, n> operator*(const double rhs, const vec<T, n>& lhs) {
    vec<T, n> ret = lhs;
    for(int i = n; i--; ret[i] *= rhs);
    return ret;
}

template<class T, int n> vec<T, n> operator*(const vec<T, n>& lhs, const double rhs) {
    vec<T, n> ret = lhs;
    for(int i = n; i--; ret[i] *= rhs);
    return ret;
}

template<class T, int n> vec<T, n> operator/(const vec<T, n>& lhs, const double rhs) {
    vec<T, n> ret = lhs;
    for(int i = n; i--; ret[i] /= rhs);
    return ret;
}

template<class T, int n> vec<T, n> operator-(const vec<T, n> v) {
    vec<T, n> ret = v;
    for(int i =n; i--; ret[i] = -ret[i]);
    return ret;
}

template<class T, int n1, int n2> vec<T, n1> embed(const vec<T, n2>& v, T fill = 1) {
    vec<T, n1> ret;
    for(int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
    return ret;
}

template<class T, int n1, int n2> vec<T, n1> proj(const vec<T, n2>& v) {
    vec<T, n1> ret;
    for(int i = n1; i--; ret[i] = v[i]);
    return ret;
}

template<class T> vec<T, 3> cross(const vec<T, 3>& v1, const vec<T, 3>& v2) {
    return vec<T, 3>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template<class T, int n> std::ostream& operator<<(std::ostream& out, const vec<T, n>& v) {
    for(int i=0; i < n; i++) {
        out << v[i] << " ";
    }
    return out;
}

template<class T1, class T2, int n> vec<T2, n> cast(const vec<T1, n>& v) {
    vec<T2, n> ret;
    for(int i = 0; i < n; i++) {
        ret[i] = (T2)(v[i]);
    }
    return ret;
}

template<class T> struct vec<T, 2> 
{
    vec() = default;
    vec(T X, T Y): x(X), y(Y){}
    T& operator[](const int i) {
        assert(i >=0 && i < 2);
        return i == 0 ? x : y;
    }
    T operator[](const int i) const {
       assert(i >=0 && i < 2);
       return i == 0 ? x : y;
    }
    double norm2() const { return (*this)*(*this); }
    double norm() const {return std::sqrt(norm2()); }
    vec& normalize() {
        *this = *this / norm(); 
        return *this;
    }
    T x, y;
};

template<class T> struct vec<T, 3>
{
    vec() = default;
    vec(T X, T Y, T Z): x(X), y(Y), z(Z){}
    T& operator[](const int i){
        assert(i >= 0 && i < 3);
        return i == 0 ? x : (i == 1) ? y : z;
    }
    T operator[](const int i) const{
        assert(i >= 0 && i < 3);
        return i == 0 ? x : (i == 1) ? y : z;
    }
    double norm2() const { return (*this)*(*this); }
    double norm() const { return std::sqrt(norm2()); }
    vec& normalize() {
        *this = *this / norm(); 
        return *this;
    }

    vec<T, 2> to2d() {
        return vec<T, 2>(x, y);
    }

    T x, y, z;
};

template<class T> struct vec<T, 4>
{
    vec() = default;
    vec(T X, T Y, T Z, T W)
        :x(X), y(Y), z(Z), w(W){}
    T operator[](const int i) const {
        assert(i >= 0 && i < 4);
        return i == 0 ? x :
                    i == 1 ? y :
                        i == 2 ? z : w;

    }

    T& operator[](const int i) {
        assert(i >= 0 && i < 4);
        return i == 0 ? x :
                    i == 1 ? y :
                        i == 2 ? z : w;

    }

    double norm2() const { return (*this)*(*this); }
    double norm() const { return std::sqrt(norm2()); }
    vec& normalize() {
        *this = *this / norm(); 
        return *this;
    }

    T x, y, z, w;
};



typedef vec<float, 2> vec2f;
typedef vec<float, 3> vec3f;
typedef vec<float, 4> vec4f;

typedef vec<int, 2> vec2i;
typedef vec<int, 3> vec3i;
typedef vec<int, 4> vec4i;

typedef vec<double, 2> vec2d;
typedef vec<double, 3> vec3d;
typedef vec<double, 4> vec4d;

    
}
