#pragma once

#include "vector.hpp"

namespace math
{
    //------------------------------define matrix------------------------------------

    template<class T, int n> struct dt; // 计算行列式所需要的

    template<class T, int nrows, int ncols> struct mat
    {
        vec<T, ncols> rows[nrows] = {{}};
        mat() = default;
        vec<T, ncols>& operator[](const int idx) {
            assert(idx >= 0 && idx < nrows);
            return rows[idx];
        }
        const vec<T, ncols>& operator[](const int idx) const {
            assert(idx >= 0 && idx < nrows);
            return rows[idx];
        }

        vec<T, nrows> col(const int idx) const {
            assert(idx >= 0 && idx < ncols);
            vec<T, nrows> ret;
            for(int i = nrows; i--; ret[i] = rows[i][idx]);
            return ret;
        }

        void set_col(const int idx, const vec<T, nrows>& v) {
            assert(idx >= 0 && idx < ncols);
            for(int i = nrows; i--; rows[i][idx]=v[i]);
        }

        static mat<T, nrows, ncols> identity() {
            mat<T, nrows, ncols> ret;
            for(int i=0; i < nrows; i++) 
                for(int j = 0; j < ncols; j++)
                    if(i == j) ret[i][j] = 1;
                    else ret[i][j] = 0;
            return ret;
        }

        double det() const {
            return dt<T, ncols>::det(*this);
        }

        mat<T, nrows-1,ncols-1> get_minor(const int row, const int col) const {
            mat<T, nrows-1,ncols-1> ret;
            for (int i=nrows-1; i--; )
                for (int j=ncols-1;j--; ret[i][j]=rows[i<row?i:i+1][j<col?j:j+1]);
            return ret;
        }

        double cofactor(const int row, const int col) const {
            return get_minor(row,col).det()*((row+col)%2 ? -1 : 1);
        }

        mat<T, nrows,ncols> adjugate() const {
            mat<T, nrows,ncols> ret;
            for (int i=nrows; i--; )
                for (int j=ncols; j--; ret[i][j]=cofactor(i,j));
            return ret;
        }

        mat<T, nrows,ncols> invert_transpose() const {
            mat<T, nrows,ncols> ret = adjugate();
            return ret/(ret[0]*rows[0]);
        }

        mat<T, nrows,ncols> invert() const {
            return invert_transpose().transpose();
        }

        mat<T, ncols,nrows> transpose() const {
            mat<T, ncols,nrows> ret;
            for (int i=ncols; i--; ret[i]=this->col(i));
            return ret;
        }

    };

    template<class T, int nrows,int ncols> vec<T, nrows> operator*(const mat<T, nrows,ncols>& lhs, const vec<T, ncols>& rhs) {
        vec<T, nrows> ret;
        for (int i=nrows; i--; ret[i]=lhs[i]*rhs);
        return ret;
    }

    template<class T, int R1,int C1,int C2>mat<T, R1,C2> operator*(const mat<T, R1,C1>& lhs, const mat<T, C1,C2>& rhs) {
        mat<T, R1,C2> result;
        for (int i=R1; i--; )
            for (int j=C2; j--; result[i][j]=lhs[i]*rhs.col(j));
        return result;
    }

    template<class T, int nrows,int ncols>mat<T, nrows,ncols> operator*(const mat<T, nrows,ncols>& lhs, const double& val) {
        mat<T, nrows,ncols> result;
        for (int i=nrows; i--; result[i] = lhs[i]*val);
        return result;
    }

    template<class T, int nrows,int ncols>mat<T, nrows,ncols> operator/(const mat<T, nrows,ncols>& lhs, const double& val) {
        mat<T, nrows,ncols> result;
        for (int i=nrows; i--; result[i] = lhs[i]/val);
        return result;
    }

    template<class T, int nrows,int ncols>mat<T, nrows, ncols> operator+(const mat<T, nrows,ncols>& lhs, const mat<T, nrows,ncols>& rhs) {
        mat<T, nrows,ncols> result;
        for (int i=nrows; i--; )
            for (int j=ncols; j--; result[i][j]=lhs[i][j]+rhs[i][j]);
        return result;
    }

    template<class T, int nrows,int ncols>mat<T, nrows,ncols> operator-(const mat<T, nrows,ncols>& lhs, const mat<T, nrows,ncols>& rhs) {
        mat<T, nrows,ncols> result;
        for (int i=nrows; i--; )
            for (int j=ncols; j--; result[i][j]=lhs[i][j]-rhs[i][j]);
        return result;
    }

    template<class T, int nrows,int ncols> std::ostream& operator<<(std::ostream& out, const mat<T, nrows,ncols>& m) {
        for (int i=0; i<nrows; i++) out << m[i] << std::endl;
        return out;
    }

    template<class T, int n> struct dt {
        static double det(const mat<T, n, n>& src) {
            double ret = 0;
            for (int i=n; i--; ret += src[0][i]*src.cofactor(0,i));
            return ret;
        }
    };

    template<class T> struct dt<T, 1> {
        static double det(const mat<T, 1,1>& src) {
            return src[0][0];
        }
    };

    typedef mat<double, 4, 4> mat4d;
    typedef mat<float, 4, 4> mat4f;
    typedef mat<int, 4, 4> mat4i;

    typedef mat<double, 3, 3> mat3d;
    typedef mat<float, 3, 3> mat3f;
    typedef mat<int, 3, 3> mat3i;

    typedef mat<double, 2, 2> mat2d;
    typedef mat<float, 2, 2> mat2f;
    typedef mat<int, 2, 2> mat2i;
} 


