/******************************************************************************
* File: simplexNoise.cpp
* Author: Tony Skeps
* Date: March 18, 2023
******************************************************************************/

#include <cmath>
#include "simplexNoise.hpp"

#include <iostream>

namespace Game::Logic::World::Generation::Noise {

    SimplexNoise::SimplexNoise(unsigned int seed) {

        // TODO implement the Strategy Pattern for Random
        srandom(seed);
        
        for (int i = 0; i < 256; i++) {
            randomValues[i] = i;
        }
        for (int i = 0; i < 255; i++) {
            unsigned int indexToSwap = random();
            indexToSwap %= (255 - i);
            indexToSwap += i;
            unsigned char tempValue = randomValues[i];
            randomValues[i] = randomValues[indexToSwap];
            randomValues[indexToSwap] = tempValue;
        }
    }

    struct grad2 {
        int x, y;
        grad2(int x, int y): x(x), y(y) {}
    } grad2s[] {
        grad2(-1, -1),
        grad2(1, -1),
        grad2(-1, 1),
        grad2(1, 1),
        grad2(-1, 0),
        grad2(1, 0),
        grad2(0, -1),
        grad2(0, 1),
        grad2(-1, 0),
        grad2(1, 0),
        grad2(0, -1),
        grad2(0, 1)
    };

    SimplexNoise2D::SimplexNoise2D(unsigned int seed): SimplexNoise(seed) {
        const int numGrad2s = sizeof(grad2s) / sizeof(grad2);
        for (int i = 0; i < 256; i++) {
            gradModRandomValues[i] = randomValues[i] % numGrad2s;
        }
    }

    float SimplexNoise2D::getNoise(float x, float y) {

        const float ScewingFactor2D = 0.5f * (sqrtf(3.0f) - 1.0f);
        const float UnscewingFactor2D = (3.0f - sqrtf(3.0f)) / 6.0f;

        float scew = (x + y) * ScewingFactor2D;
        float scewedX = x + scew;
        float scewedY = y + scew;
        int xPos = (int)scewedX;
        if (xPos > scewedX) xPos--;
        int yPos = (int)scewedY;
        if (yPos > scewedY) yPos--;

        unsigned char indexX = xPos;
        unsigned char indexY = yPos;

        struct corner {
            float x, y;
            unsigned char gradIndex;
        } corners[3];

        float unscew = (xPos + yPos) * UnscewingFactor2D;
        corners[0].x = x - (xPos - unscew);
        corners[0].y = y - (yPos - unscew);
        // why not corners[0].gradIndex = gradModRandomValues[indexX + randomValues[indexY]];?
        // For some reason, despite everything being unsigned chars, indexX + randomValues[indexY] can be greater than 255
        corners[0].gradIndex = indexY;
        corners[0].gradIndex = indexX + randomValues[corners[0].gradIndex];
        corners[0].gradIndex = gradModRandomValues[corners[0].gradIndex];

        unsigned char offsetX, offsetY;
        if (corners[0].x > corners[0].y) {
            offsetX = 1;
            offsetY = 0;
        } else {
            offsetX = 0;
            offsetY = 1;
        }
        corners[1].x = corners[0].x - offsetX + UnscewingFactor2D;
        corners[1].y = corners[0].y - offsetY + UnscewingFactor2D;
        corners[1].gradIndex = indexY + offsetY;
        corners[1].gradIndex = indexX + offsetX + randomValues[corners[1].gradIndex];
        corners[1].gradIndex = gradModRandomValues[corners[1].gradIndex];
        corners[2].x = corners[0].x - 1.0f + 2.0f * UnscewingFactor2D;
        corners[2].y = corners[0].y - 1.0f + 2.0f * UnscewingFactor2D;
        corners[2].gradIndex = indexY + 1;
        corners[2].gradIndex = indexX + 1 + randomValues[corners[2].gradIndex];
        corners[2].gradIndex = gradModRandomValues[corners[2].gradIndex];

        float result = 0.0f;

        const int numCorners = sizeof(corners) / sizeof(corner);
        for (int i = 0; i < numCorners; i++) {
            corner& c = corners[i];
            grad2& g = grad2s[c.gradIndex];
            float multiplier = 0.5f - (c.x * c.x + c.y * c.y);
            if (multiplier > 0.0f) {
                multiplier *= multiplier;
                multiplier *= multiplier;
                float addition = c.x * g.x + c.y * g.y;
                result += multiplier * addition;
            }
        }

        return result * 70.0f;
    }

    struct grad3 {
        int x, y, z;
        grad3(int x, int y, int z): x(x), y(y), z(z) {}
    } grad3s[] {
        grad3(0, -1, -1),
        grad3(0, -1, 1),
        grad3(0, 1, -1),
        grad3(0, 1, 1),
        grad3(-1, 0, -1),
        grad3(-1, 0, 1),
        grad3(1, 0, -1),
        grad3(1, 0, 1),
        grad3(-1, -1, 0),
        grad3(-1, 1, 0),
        grad3(1, -1, 0),
        grad3(1, 1, 0)
    };

    SimplexNoise3D::SimplexNoise3D(unsigned int seed): SimplexNoise(seed) {
        const int numGrad3s = sizeof(grad3s) / sizeof(grad3);
        for (int i = 0; i < 256; i++) {
            gradModRandomValues[i] = randomValues[i] % numGrad3s;
        }
    }

    float SimplexNoise3D::getNoise(float x, float y, float z) {

        const float ScewingFactor3D = 1.0f/3.0f;
        const float UnscewingFactor3D = 1.0f/6.0f;

        float scew = (x + y + z) * ScewingFactor3D;
        float scewedX = x + scew;
        float scewedY = y + scew;
        float scewedZ = z + scew;
        int xPos = (int)scewedX;
        if (xPos > scewedX) xPos--;
        int yPos = (int)scewedY;
        if (yPos > scewedY) yPos--;
        int zPos = (int)scewedZ;
        if (zPos > scewedZ) zPos--;

        unsigned char indexX = xPos;
        unsigned char indexY = yPos;
        unsigned char indexZ = zPos;

        struct corner {
            float x, y, z;
            unsigned char gradIndex;
        } corners[4];

        float unscew = (xPos + yPos + zPos) * UnscewingFactor3D;
        corners[0].x = x - (xPos - unscew);
        corners[0].y = y - (yPos - unscew);
        corners[0].z = z - (zPos - unscew);
        // why not corners[0].gradIndex = gradModRandomValues[indexX + randomValues[indexY + randomValues[indexZ]];?
        // For some reason, despite everything being unsigned chars, indexX + randomValues[indexY] can be greater than 255
        corners[0].gradIndex = indexZ;
        corners[0].gradIndex = indexY + randomValues[corners[0].gradIndex];
        corners[0].gradIndex = indexX + randomValues[corners[0].gradIndex];
        corners[0].gradIndex = gradModRandomValues[corners[0].gradIndex];

        unsigned char offset1X, offset1Y, offset1Z;
        unsigned char offset2X, offset2Y, offset2Z;
        if (corners[0].x >= corners[0].y) {
            if (corners[0].y >= corners[0].z) {
                offset1X = 1;
                offset1Y = 0;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 1;
                offset2Z = 0; // X Y Z order
            } else if (corners[0].x >= corners[0].z) {
                offset1X = 1;
                offset1Y = 0;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 0;
                offset2Z = 1; // X Z Y order
            } else {
                offset1X = 0;
                offset1Y = 0;
                offset1Z = 1;
                offset2X = 1;
                offset2Y = 0;
                offset2Z = 1;
            } // Z X Y order
        } else { // x0<y0
            if (corners[0].y < corners[0].z) {
                offset1X = 0;
                offset1Y = 0;
                offset1Z = 1;
                offset2X = 0;
                offset2Y = 1;
                offset2Z = 1; // Z Y X order
            } else if (corners[0].x < corners[0].z) {
                offset1X = 0;
                offset1Y = 1;
                offset1Z = 0;
                offset2X = 0;
                offset2Y = 1;
                offset2Z = 1; // Y Z X order
            } else {
                offset1X = 0;
                offset1Y = 1;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 1;
                offset2Z = 0;
            } // Y X Z order
        }
        corners[1].x = corners[0].x - offset1X + UnscewingFactor3D;
        corners[1].y = corners[0].y - offset1Y + UnscewingFactor3D;
        corners[1].z = corners[0].z - offset1Z + UnscewingFactor3D;
        corners[1].gradIndex = indexZ + offset1Z;
        corners[1].gradIndex = indexY + offset1Y + randomValues[corners[1].gradIndex];
        corners[1].gradIndex = indexX + offset1X + randomValues[corners[1].gradIndex];
        corners[1].gradIndex = gradModRandomValues[corners[1].gradIndex];
        corners[2].x = corners[0].x - offset2X + 2.0f * UnscewingFactor3D;
        corners[2].y = corners[0].y - offset2Y + 2.0f * UnscewingFactor3D;
        corners[2].z = corners[0].z - offset2Z + 2.0f * UnscewingFactor3D;
        corners[2].gradIndex = indexZ + offset2Z;
        corners[2].gradIndex = indexY + offset2Y + randomValues[corners[2].gradIndex];
        corners[2].gradIndex = indexX + offset2X + randomValues[corners[2].gradIndex];
        corners[2].gradIndex = gradModRandomValues[corners[2].gradIndex];
        corners[3].x = corners[0].x - 1.0f + 3.0f * UnscewingFactor3D;
        corners[3].y = corners[0].y - 1.0f + 3.0f * UnscewingFactor3D;
        corners[3].z = corners[0].z - 1.0f + 3.0f * UnscewingFactor3D;
        corners[3].gradIndex = indexZ + 1;
        corners[3].gradIndex = indexY + 1 + randomValues[corners[3].gradIndex];
        corners[3].gradIndex = indexX + 1 + randomValues[corners[3].gradIndex];
        corners[3].gradIndex = gradModRandomValues[corners[3].gradIndex];

        float result = 0.0f;

        const int numCorners = sizeof(corners) / sizeof(corner);
        for (int i = 0; i < numCorners; i++) {
            corner& c = corners[i];
            grad3& g = grad3s[c.gradIndex];
            float multiplier = 0.5f - (c.x * c.x + c.y * c.y + c.z * c.z);
            if (multiplier > 0.0f) {
                multiplier *= multiplier;
                multiplier *= multiplier;
                float addition = c.x * g.x + c.y * g.y + c.z * g.z;
                result += multiplier * addition;
            }
        }

        return result * 70.0f;
    }

    struct grad4 {
        int x, y, z, w;
        grad4(int x, int y, int z): x(x), y(y), z(z) {}
    } grad4s[] {
        grad4(0, -1, -1),
        grad4(0, -1, 1),
        grad4(0, 1, -1),
        grad4(0, 1, 1),
        grad4(-1, 0, -1),
        grad4(-1, 0, 1),
        grad4(1, 0, -1),
        grad4(1, 0, 1),
        grad4(-1, -1, 0),
        grad4(-1, 1, 0),
        grad4(1, -1, 0),
        grad4(1, 1, 0)
    };

    SimplexNoise4D::SimplexNoise4D(unsigned int seed): SimplexNoise(seed) {
        const int numGrad4s = sizeof(grad4s) / sizeof(grad4);
        for (int i = 0; i < 256; i++) {
            gradModRandomValues[i] = randomValues[i] % numGrad4s;
        }
    }

    float SimplexNoise4D::getNoise(float x, float y, float z, float w) {

        const float ScewingFactor4D = 1.0f/3.0f;
        const float UnscewingFactor4D = 1.0f/6.0f;

        float scew = (x + y + z + w) * ScewingFactor4D;
        float scewedX = x + scew;
        float scewedY = y + scew;
        float scewedZ = z + scew;
        float scewedW = w + scew;
        int xPos = (int)scewedX;
        if (xPos > scewedX) xPos--;
        int yPos = (int)scewedY;
        if (yPos > scewedY) yPos--;
        int zPos = (int)scewedZ;
        if (zPos > scewedZ) zPos--;
        int wPos = (int)scewedW;
        if (wPos > scewedW) wPos--;

        unsigned char indexX = xPos;
        unsigned char indexY = yPos;
        unsigned char indexZ = zPos;
        unsigned char indexW = wPos;

        struct corner {
            float x, y, z, w;
            unsigned char gradIndex;
        } corners[5];

        float unscew = (xPos + yPos + zPos) * UnscewingFactor4D;
        corners[0].x = x - (xPos - unscew);
        corners[0].y = y - (yPos - unscew);
        corners[0].z = z - (zPos - unscew);
        // why not corners[0].gradIndex = gradModRandomValues[indexX + randomValues[indexY + randomValues[indexZ]];?
        // For some reason, despite everything being unsigned chars, indexX + randomValues[indexY] can be greater than 255
        corners[0].gradIndex = indexZ;
        corners[0].gradIndex = indexY + randomValues[corners[0].gradIndex];
        corners[0].gradIndex = indexX + randomValues[corners[0].gradIndex];
        corners[0].gradIndex = gradModRandomValues[corners[0].gradIndex];

        unsigned char offset1X, offset1Y, offset1Z;
        unsigned char offset2X, offset2Y, offset2Z;
        if (corners[0].x >= corners[0].y) {
            if (corners[0].y >= corners[0].z) {
                offset1X = 1;
                offset1Y = 0;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 1;
                offset2Z = 0; // X Y Z order
            } else if (corners[0].x >= corners[0].z) {
                offset1X = 1;
                offset1Y = 0;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 0;
                offset2Z = 1; // X Z Y order
            } else {
                offset1X = 0;
                offset1Y = 0;
                offset1Z = 1;
                offset2X = 1;
                offset2Y = 0;
                offset2Z = 1;
            } // Z X Y order
        } else { // x0<y0
            if (corners[0].y < corners[0].z) {
                offset1X = 0;
                offset1Y = 0;
                offset1Z = 1;
                offset2X = 0;
                offset2Y = 1;
                offset2Z = 1; // Z Y X order
            } else if (corners[0].x < corners[0].z) {
                offset1X = 0;
                offset1Y = 1;
                offset1Z = 0;
                offset2X = 0;
                offset2Y = 1;
                offset2Z = 1; // Y Z X order
            } else {
                offset1X = 0;
                offset1Y = 1;
                offset1Z = 0;
                offset2X = 1;
                offset2Y = 1;
                offset2Z = 0;
            } // Y X Z order
        }
        corners[1].x = corners[0].x - offset1X + UnscewingFactor4D;
        corners[1].y = corners[0].y - offset1Y + UnscewingFactor4D;
        corners[1].z = corners[0].z - offset1Z + UnscewingFactor4D;
        corners[1].gradIndex = indexZ + offset1Z;
        corners[1].gradIndex = indexY + offset1Y + randomValues[corners[1].gradIndex];
        corners[1].gradIndex = indexX + offset1X + randomValues[corners[1].gradIndex];
        corners[1].gradIndex = gradModRandomValues[corners[1].gradIndex];
        corners[2].x = corners[0].x - offset2X + 2.0f * UnscewingFactor4D;
        corners[2].y = corners[0].y - offset2Y + 2.0f * UnscewingFactor4D;
        corners[2].z = corners[0].z - offset2Z + 2.0f * UnscewingFactor4D;
        corners[2].gradIndex = indexZ + offset2Z;
        corners[2].gradIndex = indexY + offset2Y + randomValues[corners[2].gradIndex];
        corners[2].gradIndex = indexX + offset2X + randomValues[corners[2].gradIndex];
        corners[2].gradIndex = gradModRandomValues[corners[2].gradIndex];
        corners[3].x = corners[0].x - 1.0f + 3.0f * UnscewingFactor4D;
        corners[3].y = corners[0].y - 1.0f + 3.0f * UnscewingFactor4D;
        corners[3].z = corners[0].z - 1.0f + 3.0f * UnscewingFactor4D;
        corners[3].gradIndex = indexZ + 1;
        corners[3].gradIndex = indexY + 1 + randomValues[corners[3].gradIndex];
        corners[3].gradIndex = indexX + 1 + randomValues[corners[3].gradIndex];
        corners[3].gradIndex = gradModRandomValues[corners[3].gradIndex];

        float result = 0.0f;

        const int numCorners = sizeof(corners) / sizeof(corner);
        for (int i = 0; i < numCorners; i++) {
            corner& c = corners[i];
            grad3& g = grad3s[c.gradIndex];
            float multiplier = 0.5f - (c.x * c.x + c.y * c.y + c.z * c.z);
            if (multiplier > 0.0f) {
                multiplier *= multiplier;
                multiplier *= multiplier;
                float addition = c.x * g.x + c.y * g.y + c.z * g.z;
                result += multiplier * addition;
            }
        }

        return result * 70.0f;
    }

}