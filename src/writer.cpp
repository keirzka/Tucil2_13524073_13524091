#include "writer.hpp"
#include <algorithm>
#include <iostream>

bool compareVec3(const vec3 &a, const vec3 &b)
{
    if (a.x != b.x)
        return a.x < b.x;
    if (a.y != b.y)
        return a.y < b.y;
    return a.z < b.z;
}

// Fungsi komparator untuk mengecek kesamaan koordinat secara presisi
bool equalVec3(const vec3 &a, const vec3 &b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// Mengumpulkan semua nodes leaf daro octree
void collectLeafNodes(OctreeNode* node, std::vector<OctreeNode*>& leaves) {
    if (!node) return;

    if (node->isLeaf) {
        leaves.push_back(node);
        return;
    }

    for (int i = 0; i < 8; i++) {
        collectLeafNodes(node->children[i], leaves);
    }
}

// Membuat 8 vertex dari AABB
std::vector<vec3> generateCubeVertices(const AABB &box)
{
    vec3 min = box.min;
    vec3 max = box.max;

    return {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {min.x, max.y, min.z},
        {max.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {min.x, max.y, max.z},
        {max.x, max.y, max.z}
    };
}

// Membuat 12 face triangle dari kubus
std::vector<std::array<int, 3>> generateCubeFaces(const int idx[8])
{
    int v1 = idx[0], v2 = idx[1], v3 = idx[2], v4 = idx[3];
    int v5 = idx[4], v6 = idx[5], v7 = idx[6], v8 = idx[7];

    return {
        {v1, v3, v2}, {v2, v3, v4}, // Bawah
        {v5, v6, v7},
        {v6, v8, v7}, // Atas
        {v1, v5, v3},
        {v3, v5, v7}, // Kiri
        {v2, v4, v6},
        {v4, v8, v6}, // Kanan
        {v1, v2, v5},
        {v2, v6, v5}, // Depan
        {v3, v7, v4},
        {v4, v7, v8} // Belakang
    };
}

// Write to file objek
void writeOBJ(const std::string &filename, const std::vector<OctreeNode *> &leaves)
{
    std::ofstream file(filename);

    // Perbaikan Pengecekan File
    if (!file.is_open())
    {
        std::cerr << "Error: Tidak dapat membuka atau membuat file " << filename << "\n";
        return;
    }

    std::vector<vec3> allVertices;

    // Kumpulkan semua vertex
    for (OctreeNode *node : leaves)
    {
        std::vector<vec3> vertices = generateCubeVertices(node->bounds);
        allVertices.insert(allVertices.end(), vertices.begin(), vertices.end());
    }

    // Deduplikasi menggunakan sorting
    std::vector<vec3> uniqueVertices = allVertices;
    sort(uniqueVertices.begin(), uniqueVertices.end(), compareVec3);
    auto last = unique(uniqueVertices.begin(), uniqueVertices.end(), equalVec3);
    uniqueVertices.erase(last, uniqueVertices.end());

    // Tulis daftar vertex unik ke file (.obj)
    for (const vec3 &v : uniqueVertices)
    {
        file << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }

    // Pencarian indeks menggunakan Binary Search
    for (OctreeNode *node : leaves)
    {
        std::vector<vec3> vertices = generateCubeVertices(node->bounds);
        int currentIndices[8];

        for (int i = 0; i < 8; i++)
        {
            // Cari posisi vertex di array unik yang sudah terurut
            auto it = lower_bound(uniqueVertices.begin(), uniqueVertices.end(), vertices[i], compareVec3);

            // Format .obj menggunakan indeks berbasis 1 (bukan 0)
            currentIndices[i] = distance(uniqueVertices.begin(), it) + 1;
        }

        std::vector<std::array<int, 3>> faces = generateCubeFaces(currentIndices);
        for (const auto &f : faces)
        {
            file << "f " << f[0] << " " << f[1] << " " << f[2] << "\n";
        }
    }

    file.close();
}