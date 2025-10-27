#pragma once
#include "../../MyLib/Utilities/BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"

inline uint32_t ColorVectorToUint32(const Vector4<float>& color)
{
    uint32_t r = static_cast<uint32_t>(color.x * 255.0f) & 0xFF;
    uint32_t g = static_cast<uint32_t>(color.y * 255.0f) & 0xFF;
    uint32_t b = static_cast<uint32_t>(color.z * 255.0f) & 0xFF;
    uint32_t a = static_cast<uint32_t>(color.w * 255.0f) & 0xFF;

    return (r << 24) | (g << 16) | (b << 8) | a;
}

inline Vector4<float> Uint32ToColorVector(uint32_t color)
{
    float r = ((color >> 24) & 0xFF) / 255.0f;
    float g = ((color >> 16) & 0xFF) / 255.0f;
    float b = ((color >> 8) & 0xFF) / 255.0f;
    float a = (color & 0xFF) / 255.0f;

    return { r, g, b, a };
}


inline Vector3 Lerp(Vector3 a, Vector3 b, float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

inline static float Distance(const Vector3& a, const Vector3& b)
{
    Vector3 diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

inline int GetRand(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

inline float GetRand(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
