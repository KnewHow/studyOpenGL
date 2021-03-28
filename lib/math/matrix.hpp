#pragma once

#include "vector.hpp"
#include <cmath>

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

    /**
     * translate matrix
     * @param x the offset of x
     * @param y the offset of y
     * @param z the offset of z
     * @return the translate matrix
    */
    static inline mat4f translate(float x, float y, float z) {
        mat4f res;
        res[0] = vec4f(1.0f, 0.0f, 0.0f, x);
        res[1] = vec4f(0.0f, 1.0f, 0.0f, y);
        res[2] = vec4f(0.0f, 0.0f, 1.0f, z);
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    static inline float angleToRadian(float angle) {
        return angle * M_PI / 180.0f;
    }

    /**
     * rotate by x-axis
     * @param x the angle of rotate
    */
    static inline mat4f rotateX(float x) {
        float radian = angleToRadian(x);
        mat4f res;
        res[0] = vec4f(1.0f, 0.0f, 0.0f, 0.0f);
        res[1] = vec4f(0.0f, std::cos(radian), -std::sin(radian), 0.0f);
        res[2] = vec4f(0.0f, std::sin(radian), std::cos(radian), 0.0f);
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    /**
     * rotate by y-axis
     * @param y the angle of rotate
    */
    static inline mat4f rotateY(float y) {
        float radian = angleToRadian(y);
        mat4f res;
        res[0] = vec4f(std::cos(radian), 0.0f, std::sin(radian), 0.0f);
        res[1] = vec4f(0.0f, 1.0f, 0.0f, 0.0f);
        res[2] = vec4f(-std::sin(radian), 0.0f, std::cos(radian), 0.0f);
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    /**
     * rotate by z-axis
     * @param y the angle of rotate
    */
    static inline mat4f rotateZ(float z) {
        float radian = angleToRadian(z);
        mat4f res;
        res[0] = vec4f(std::cos(radian), -std::sin(radian), 0.0f, 0.0f);
        res[1] = vec4f(std::sin(radian), std::cos(radian), 0.0f, 0.0f);
        res[2] = vec4f(0.0f, 0.0f, 1.0f, 0.0f);
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    /**
     * a rotate matrix with rotate with coordinate axis, all paramater is angle
     * @param x the rotate of x-axis
     * @param y the rotate of y-axis
     * @param z the rotate of z-axis
     * @return the rotate matrix
    */
    static inline mat4f rotate(float x, float y, float z) {
        return rotateX(x) * rotateY(y) * rotateZ(z);
    }

    /**
     * scala by x-axis, y-axis and z-axis, if you only scale one axis, let others is 1.0f
     * @param x the scale of x-axis
     * @param y the scale of y-axis
     * @param z the scale of z-axis
     * @return the scale matrix
    */
    static inline mat4f scale(float x, float y, float z) {
        mat4f res;
        res[0] = vec4f(x, 0.0f, 0.0f, 0.0f);
        res[1] = vec4f(0.0f, y, 0.0f, 0.0f);
        res[2] = vec4f(0.0f, 0.0f, z, 0.0f);
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    /**
     * get orthogonal projection matrix
     * @param left the left border of orthogonal box
     * @param right the right border of orthogonal box
     * @param bottom  the bottom border of the orthogonal box
     * @param top the top border of the orthogonal box
     * @param near the zear plane distance of orthogonal box, the value is positive
     * @param far the far plane distance of the orthogonal box, the value is positive
     * @return the orthogonal projection matrix
    */
    static inline mat4f orthogonal(float left, float right, float bottom, float top, float near, float far) {
        mat4f res;
        res[0] = vec4f(2.0 / (right - left), 0.0f, 0.0f, (right + left) / (left -right));
        res[1] = vec4f(0.0f, 2.0f / (top - bottom), 0.0f, (top + bottom) / (bottom - left));
        res[2] = vec4f(0.0f, 0.0f, -2 / (far - near), (far + near) / (near - far));
        res[3] = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        return res;
    }

    /**
     * get perspective projection matrix
     * @param fov the field of view, it is angle
     * @param aspectRadio the ratio of screen width and height
     * @param zNear the near plane distance, this value is positive
     * @param zFar the far plane distance, this value is positive
    */
    static inline mat4f perspective(float fov, float aspectRatio, float zNear, float zFar) {
        float radian = angleToRadian(fov);
        float A = 1.0f / std::tan(radian / 2.0f);
        float B =  A / aspectRatio;
        float C = (zFar + zNear) / (zNear - zFar);
        float D = (2 * zFar * zNear) / (zNear - zFar);
        mat4f res;
        res[0] = vec4f(B, 0.0f, 0.0f, 0.0f);
        res[1] = vec4f(0.0f, A, 0.0f, 0.0f);
        res[2] = vec4f(0.0f, 0.0f, C, D);
        res[3] = vec4f(0.0f, 0.0f, -1.0f, 0.0f);
        return res;
    }
} 


