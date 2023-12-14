/******************************************************************************
* File: glMath.hpp
* Author: Tony Skeps
* Date: July 5, 2022
******************************************************************************/

#ifndef GL_MATH_HPP
#define GL_MATH_HPP

namespace Game::GL {

    struct vec2 {
        float x, y;
        vec2() {}
        vec2(float x, float y): x(x), y(y) {}
    };

    struct vec3{
        float x, y, z;
        vec3() {}
        vec3(float x, float y, float z): x(x), y(y), z(z) {}
    };

    struct vec4{
        float x, y, z, w;
        vec4() {}
        vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
    };

    struct mat2 {
        float m11, m21, m12, m22;

        /*
            m11 m12
            m21 m22
        */

        mat2() {}
        mat2(float m11, float m12, float m21, float m22): m11(m11), m12(m12), m21(m21), m22(m22) {}
    };

    struct mat3 {
        float m11, m21, m31, m12, m22, m32, m13, m23, m33;

        /*
            m11 m12 m13
            m21 m22 m23
            m31 m32 m33
        */
        
        mat3() {}
        mat3(float m11, float m12, float m13,
            float m21, float m22, float m23,
            float m31, float m32, float m33):
            m11(m11), m12(m12), m13(m13),
            m21(m21), m22(m22), m23(m23),
            m31(m31), m32(m32), m33(m33) {}
    };

    struct mat4 {
        float m11, m21, m31, m41, m12, m22, m32, m42, m13, m23, m33, m43, m14, m24, m34, m44;

        /*
            m11 m12 m13 m14
            m21 m22 m23 m24
            m31 m32 m33 m34
            m41 m42 m43 m44
        */
        
        mat4() {}
        mat4(float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44):
            m11(m11), m12(m12), m13(m13), m14(m14),
            m21(m21), m22(m22), m23(m23), m24(m24),
            m31(m31), m32(m32), m33(m33), m34(m34),
            m41(m41), m42(m42), m43(m43), m44(m44) {}
    };

    vec2 operator+(vec2, vec2);
    vec2 operator*(vec2, float);
    vec2 operator*(float, vec2);
    float dot(vec2, vec2);

    mat2 operator+(mat2, mat2);
    mat2 operator*(mat2, mat2);
    mat2 operator*(mat2, float);
    mat2 operator*(float, mat2);
    vec2 operator*(mat2, vec2);

    mat2 rotation2D(float radians);


    vec3 operator+(vec3, vec3);
    vec3 operator*(vec3, float);
    vec3 operator*(float, vec3);
    float dot(vec3, vec3);

    mat3 operator+(mat3, mat3);
    mat3 operator*(mat3, mat3);
    mat3 operator*(mat3, float);
    mat3 operator*(float, mat3);
    vec3 operator*(mat3, vec3);

    mat3 rotation(float radians);
    mat3 hyperbolicTranslation(vec2 offset);
    mat3 sphericalTranslation(vec2 offset);
    mat3 translation(vec2 offset);


    vec4 operator+(vec4, vec4);
    vec4 operator*(vec4, float);
    vec4 operator*(float, vec4);
    float dot(vec4, vec4);

    mat4 operator+(mat4, mat4);
    mat4 operator*(mat4, mat4);
    mat4 operator*(mat4, float);
    mat4 operator*(float, mat4);
    vec4 operator*(mat4, vec4);

    mat4 axisAngle(vec3 axis, float angle);
    mat4 hyperbolicTranslation(vec3 offset);
    mat4 sphericalTranslation(vec3 offset);
    mat4 translation(vec3 offset);
    mat4 frustum(float left, float right, float bottom, float top, float near, float far);
    mat4 perspective(float fov, float aspectRatio, float near, float far);
    mat4 rotationX(float radians);
    mat4 rotationY(float radians);
    mat4 rotationZ(float radians);

}

#endif