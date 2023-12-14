/******************************************************************************
* File: glMath.cpp
* Author: Tony Skeps
* Date: August 28, 2022
******************************************************************************/

#include <cmath>
#include "glMath.hpp"

namespace Game::GL {

    vec2 operator+(vec2 lhs, vec2 rhs) {
        return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    vec2 operator*(vec2 lhs, float rhs) {
        return vec2(lhs.x * rhs, lhs.y * rhs);
    }

    vec2 operator*(float lhs, vec2 rhs) {
        return rhs * lhs;
    }

    float dot(vec2 lhs, vec2 rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    mat2 operator+(mat2 lhs, mat2 rhs) {
        return mat2(lhs.m11 + rhs.m11, lhs.m12 + rhs.m12,
            lhs.m21 + rhs.m21, lhs.m22 + rhs.m22);
    }

    mat2 operator*(mat2 lhs, mat2 rhs) {
        return mat2(lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21, lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22,
            lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21, lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22);
    }

    mat2 operator*(mat2 lhs, float rhs) {
        return mat2(lhs.m11 * rhs, lhs.m12 * rhs,
            lhs.m21 * rhs, lhs.m22 * rhs);
    }

    mat2 operator*(float lhs, mat2 rhs) {
        return rhs * lhs;
    }

    vec2 operator*(mat2 transform, vec2 vector) {
        return vec2(transform.m11 * vector.x + transform.m12 * vector.y,
            transform.m21 * vector.x + transform.m22 * vector.y);
    }

    mat2 rotation2D(float radians) {
        return mat2(cosf(radians), -sinf(radians),
            sinf(radians), cosf(radians));
    }


    vec3 operator+(vec3 lhs, vec3 rhs) {
        return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    vec3 operator*(vec3 lhs, float rhs) {
        return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    vec3 operator*(float lhs, vec3 rhs) {
        return rhs * lhs;
    }

    float dot(vec3 lhs, vec3 rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    vec3 cross(vec3 lhs, vec3 rhs) {
        return vec3(lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x);
    }

    mat3 operator+(mat3 lhs, mat3 rhs) {
        return mat3(lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13,
            lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23,
            lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33);
    }

    mat3 operator*(mat3 lhs, mat3 rhs) {
        return mat3(lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31, lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32, lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33,
            lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31, lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32, lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33,
            lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31, lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32, lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33);
    }

    mat3 operator*(mat3 lhs, float rhs) {
        return mat3(lhs.m11 * rhs, lhs.m12 * rhs, lhs.m13 * rhs,
            lhs.m21 * rhs, lhs.m22 * rhs, lhs.m23 * rhs,
            lhs.m31 * rhs, lhs.m32 * rhs, lhs.m33 * rhs);
    }

    mat3 operator*(float lhs, mat3 rhs) {
        return rhs * lhs;
    }

    vec3 operator*(mat3 transform, vec3 vector) {
        return vec3(transform.m11 * vector.x + transform.m12 * vector.y + transform.m13 * vector.z,
            transform.m21 * vector.x + transform.m22 * vector.y + transform.m23 * vector.z,
            transform.m31 * vector.x + transform.m32 * vector.y + transform.m33 * vector.z);
    }

    mat3 rotation(float radians) {
        return mat3(cosf(radians), -sinf(radians), 0,
            sinf(radians), cosf(radians), 0,
            0, 0, 1);
    }

    mat3 getTranslation(vec2 direction, mat2 singleAxisTranslation) {
        float A = singleAxisTranslation.m11 - 1;
        return mat3(A * direction.x * direction.x + 1, A * direction.x * direction.y, singleAxisTranslation.m12 * direction.x,
            A * direction.y * direction.x, A * direction.y * direction.y + 1, singleAxisTranslation.m12 * direction.y,
            singleAxisTranslation.m21 * direction.x, singleAxisTranslation.m21 * direction.y, singleAxisTranslation.m22);
    }

    mat3 hyperbolicTranslation(vec2 offset) {
        float magnitude = sqrtf(offset.x * offset.x + offset.y * offset.y);
        if (magnitude > 0) {
            float CosH = coshf(magnitude), SinH = sinhf(magnitude);
            return getTranslation(offset * (1 / magnitude), mat2(CosH, SinH, SinH, CosH));
        } else {
            return mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
        }
    }

    mat3 sphericalTranslation(vec2 offset) {
        float magnitude = sqrtf(offset.x * offset.x + offset.y * offset.y);
        if (magnitude > 0) {
            float Cos = cosf(magnitude), Sin = sinf(magnitude);
            return getTranslation(offset * (1 / magnitude), mat2(Cos, Sin, -Sin, Cos));
        } else {
            return mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
        }
    }

    mat3 translation(vec2 offset) {
        return mat3(1, 0, offset.x,
            0, 1, offset.y,
            0, 0, 1);
    }


    vec4 operator+(vec4 lhs, vec4 rhs) {
        return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    vec4 operator*(vec4 lhs, float rhs) {
        return vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }

    vec4 operator*(float lhs, vec4 rhs) {
        return rhs * lhs;
    }

    float dot(vec4 lhs, vec4 rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
    }

    mat4 operator+(mat4 lhs, mat4 rhs) {
        return mat4(lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13, lhs.m14 + rhs.m14,
            lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23, lhs.m24 + rhs.m24,
            lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33, lhs.m34 + rhs.m34,
            lhs.m41 + rhs.m41, lhs.m42 + rhs.m42, lhs.m43 + rhs.m43, lhs.m44 + rhs.m44);
    }

    mat4 operator*(mat4 lhs, mat4 rhs) {
        return mat4(lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 + lhs.m14 * rhs.m41, lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 + lhs.m14 * rhs.m42, lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 + lhs.m14 * rhs.m43, lhs.m11 * rhs.m14 + lhs.m12 * rhs.m24 + lhs.m13 * rhs.m34 + lhs.m14 * rhs.m44,
            lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 + lhs.m24 * rhs.m41, lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 + lhs.m24 * rhs.m42, lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 + lhs.m24 * rhs.m43, lhs.m21 * rhs.m14 + lhs.m22 * rhs.m24 + lhs.m23 * rhs.m34 + lhs.m24 * rhs.m44,
            lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 + lhs.m34 * rhs.m41, lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 + lhs.m34 * rhs.m42, lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 + lhs.m34 * rhs.m43, lhs.m31 * rhs.m14 + lhs.m32 * rhs.m24 + lhs.m33 * rhs.m34 + lhs.m34 * rhs.m44,
            lhs.m41 * rhs.m11 + lhs.m42 * rhs.m21 + lhs.m43 * rhs.m31 + lhs.m44 * rhs.m41, lhs.m41 * rhs.m12 + lhs.m42 * rhs.m22 + lhs.m43 * rhs.m32 + lhs.m44 * rhs.m42, lhs.m41 * rhs.m13 + lhs.m42 * rhs.m23 + lhs.m43 * rhs.m33 + lhs.m44 * rhs.m43, lhs.m41 * rhs.m14 + lhs.m42 * rhs.m24 + lhs.m43 * rhs.m34 + lhs.m44 * rhs.m44);
    }

    mat4 operator*(mat4 lhs, float rhs) {
        return mat4(lhs.m11 * rhs, lhs.m12 * rhs, lhs.m13 * rhs, lhs.m14 * rhs,
            lhs.m21 * rhs, lhs.m22 * rhs, lhs.m23 * rhs, lhs.m24 * rhs,
            lhs.m31 * rhs, lhs.m32 * rhs, lhs.m33 * rhs, lhs.m34 * rhs,
            lhs.m41 * rhs, lhs.m42 * rhs, lhs.m43 * rhs, lhs.m44 * rhs);
    }

    mat4 operator*(float lhs, mat4 rhs) {
        return rhs * lhs;
    }

    vec4 operator*(mat4 transform, vec4 vector) {
        return vec4(transform.m11 * vector.x + transform.m12 * vector.y + transform.m13 * vector.z + transform.m14 * vector.w,
            transform.m21 * vector.x + transform.m22 * vector.y + transform.m23 * vector.z + transform.m24 * vector.w,
            transform.m31 * vector.x + transform.m32 * vector.y + transform.m33 * vector.z + transform.m34 * vector.w,
            transform.m41 * vector.x + transform.m42 * vector.y + transform.m43 * vector.z + transform.m44 * vector.w);
    }

    mat4 axisAngle(vec3 axis, float angle) {
        float c = cosf(angle), s = sinf(angle);
        float t = 1 - c;
        return mat4(t * axis.x * axis.x + c, t * axis.x * axis.y - s * axis.z, t * axis.x * axis.z + s * axis.y, 0,
            t * axis.x * axis.y + s * axis.z, t * axis.y * axis.y + c, t * axis.y * axis.z - s * axis.x, 0,
            t * axis.x * axis.z - s * axis.y, t * axis.y * axis.z + s * axis.x, t * axis.z * axis.z + c, 0,
            0, 0, 0, 1);
    }

    mat4 getTranslation(vec3 direction, mat2 singleAxisTranslation) {
        float A = singleAxisTranslation.m11 - 1;
        return mat4(A * direction.x * direction.x + 1, A * direction.x * direction.y, A * direction.x * direction.z, singleAxisTranslation.m12 * direction.x,
            A * direction.y * direction.x, A * direction.y * direction.y + 1, A * direction.y * direction.z, singleAxisTranslation.m12 * direction.y,
            A * direction.z * direction.x, A * direction.z * direction.y, A * direction.z * direction.z + 1, singleAxisTranslation.m12 * direction.z,
            singleAxisTranslation.m21 * direction.x, singleAxisTranslation.m21 * direction.y, singleAxisTranslation.m21 * direction.z, singleAxisTranslation.m22);
    }

    mat4 hyperbolicTranslation(vec3 offset) {
        float magnitude = sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
        if (magnitude > 0) {
            float CosH = coshf(magnitude), SinH = sinhf(magnitude);
            return getTranslation(offset * (1 / magnitude), mat2(CosH, SinH, SinH, CosH));
        } else {
            return mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        }
    }

    mat4 sphericalTranslation(vec3 offset) {
        float magnitude = sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
        if (magnitude > 0) {
            float CosH = cosf(magnitude), SinH = sinf(magnitude);
            return getTranslation(offset * (1 / magnitude), mat2(CosH, SinH, -SinH, CosH));
        } else {
            return mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        }
    }

    mat4 translation(vec3 offset) {
        return mat4(1, 0, 0, offset.x,
            0, 1, 0, offset.y,
            0, 0, 1, offset.z,
            0, 0, 0, 1);
    }

    mat4 frustum(float left, float right, float bottom, float top, float near, float far) {
        return mat4(2 * near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
            0.0f, 2 * near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
            0.0f, 0.0f, -(far + near) / (far - near), -2 * far * near / (far - near),
            0.0f, 0.0f, -1.0f, 0.0f);
    }

    mat4 perspective(float fov, float aspectRatio, float near, float far) {
        float yMax = near * tanf(fov);
        float xMax = yMax * aspectRatio;
        return frustum(-xMax, xMax, -yMax, yMax, near, far); 
    }

    mat4 rotationX(float radians) {
        float c = cosf(radians), s = sinf(radians);
        return mat4(1, 0, 0, 0,
            0, c, s, 0,
            0, -s, c, 0,
            0, 0, 0, 1);
    }

    mat4 rotationY(float radians) {
        float c = cosf(radians), s = sinf(radians);
        return mat4(c, 0, -s, 0,
            0, 1, 0, 0,
            s, 0, c, 0,
            0, 0, 0, 1);
    }

    mat4 rotationZ(float radians) {
        float c = cosf(radians), s = sinf(radians);
        return mat4(c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }

}
