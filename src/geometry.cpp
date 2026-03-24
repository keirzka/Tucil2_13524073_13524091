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