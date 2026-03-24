#pragma once
#include <algorithm>
#include <vector>

struct vec3
{
    float x, y, z;
};

struct Triangle
{
    vec3 v0, v1, v2;
};

struct AABB
{
    vec3 min, max;
};

// helper aabb
inline vec3 vec3_min(const vec3 &a, const vec3 &b)
{
    return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

inline vec3 vec3_max(const vec3 &a, const vec3 &b)
{
    return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

inline vec3 aabb_center(const AABB &box)
{
    return {
        (box.max.x + box.min.x) * 0.5f,
        (box.max.y + box.min.y) * 0.5f,
        (box.max.z + box.min.z) * 0.5f,
    };
}

inline vec3 aabb_half_size(const AABB &box)
{
    return {
        (box.max.x - box.min.x) * 0.5f,
        (box.max.y - box.min.y) * 0.5f,
        (box.max.z - box.min.z) * 0.5f,
    };
}

// fungsi
AABB computeBounds(const std::vector<vec3> &vertices);

bool triangleIntersectsAABB(const Triangle &tri, const AABB &box);