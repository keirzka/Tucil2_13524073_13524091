#include "geometry.hpp"

AABB computeBounds(const std::vector<vec3> &vertices)
{
    // cari nilai min max
    vec3 mn = vertices[0];
    vec3 mx = vertices[0];

    for (const auto &v : vertices)
    {
        mn = vec3_min(mn, v);
        mx = vec3_max(mx, v);
    }

    // ambil yg paling panjang supaya jd kubus
    float sizeX = mx.x - mn.x;
    float sizeY = mx.y - mn.y;
    float sizeZ = mx.z - mn.z;
    float maxSize = std::max({sizeX, sizeY, sizeZ});

    vec3 center = {
        (mn.x + mx.x) * 0.5f,
        (mn.y + mx.y) * 0.5f,
        (mn.z + mx.z) * 0.5f};

    float half = maxSize * 0.5f;

    // ambil min max AABB
    return {
        {center.x - half, center.y - half, center.z - half},
        {center.x + half, center.y + half, center.z + half}};
}

// helper triangleIntersectsAABB, implementasi separating axis theorem (SAT)
static bool overlapOnAxis(
    const vec3 &v0, const vec3 &v1, const vec3 &v2,
    const vec3 &half, const vec3 &axis)
{
    // proyeksi vertex ke sumbu
    float p0 = v0.x * axis.x + v0.y * axis.y + v0.z * axis.z;
    float p1 = v1.x * axis.x + v1.y * axis.y + v1.z * axis.z;
    float p2 = v2.x * axis.x + v2.y * axis.y + v2.z * axis.z;

    float minP = std::min({p0, p1, p2});
    float maxP = std::max({p0, p1, p2});

    float r = half.x * std::abs(axis.x) +
              half.y * std::abs(axis.y) +
              half.z * std::abs(axis.z);

    return !(minP > r || maxP < -r);
}

static vec3 cross(const vec3 &a, const vec3 &b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

// cari intersect untuk bantu bagian rekursi nanti.
// false -> nodenya merupakan ruang kosong sehingga bisa dipruning
// true dan depth < maxDepth -> node harus dipecah lagi (rekursi)
// trua dan depth == maxDepth -> leaf voxel (rekursi berhenti)
bool triangleIntersectsAABB(const Triangle &tri, const AABB &box)
{
    vec3 center = aabb_center(box);
    vec3 half = aabb_half_size(box);

    vec3 v0 = {tri.v0.x - center.x, tri.v0.y - center.y, tri.v0.z - center.z};
    vec3 v1 = {tri.v1.x - center.x, tri.v1.y - center.y, tri.v1.z - center.z};
    vec3 v2 = {tri.v2.x - center.x, tri.v2.y - center.y, tri.v2.z - center.z};

    vec3 e0 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
    vec3 e1 = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    vec3 e2 = {v0.x - v2.x, v0.y - v2.y, v0.z - v2.z};

    vec3 axes[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    // 3 sumbu AABB
    for (auto &a : axes)
        if (!overlapOnAxis(v0, v1, v2, half, a))
            return false;

    // 1 sumbu normal segitiga
    vec3 normal = cross(e0, e1);
    if (!overlapOnAxis(v0, v1, v2, half, normal))
        return false;

    // 9 sumbu cross product
    vec3 edges[3] = {e0, e1, e2};
    for (auto &e : edges)
        for (auto &a : axes)
            if (!overlapOnAxis(v0, v1, v2, half, cross(e, a)))
                return false;

    return true;
}