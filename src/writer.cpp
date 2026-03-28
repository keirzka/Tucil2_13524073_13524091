#include "writer.hpp"

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
std::vector<std::array<int, 3>> generateCubeFaces(int offset)
{
    int v1 = offset + 1, v2 = offset + 2, v3 = offset + 3, v4 = offset + 4;
    int v5 = offset + 5, v6 = offset + 6, v7 = offset + 7, v8 = offset + 8;

    return {
        // Face Bawah (-Z)
        {v1, v3, v2},
        {v2, v3, v4},
        // Face Atas (+Z)
        {v5, v6, v7},
        {v6, v8, v7},
        // Face Kiri (-X)
        {v1, v5, v3},
        {v3, v5, v7},
        // Face Kanan (+X)
        {v2, v4, v6},
        {v4, v8, v6},
        // Face Depan (-Y)
        {v1, v2, v5},
        {v2, v6, v5},
        // Face Belakang (+Y)
        {v3, v7, v4},
        {v4, v7, v8}};
}

// Write to file objek
void writeOBJ(const std::string &filename, const std::vector<OctreeNode *> &leaves)
{
    std::ofstream file(filename);

    int vertexOffset = 0;

    for (OctreeNode* node : leaves) {

        std::vector<vec3> vertices = generateCubeVertices(node->bounds);

        // tulis vertex
        for (vec3 v : vertices) {
            file << "v " << v.x << " " << v.y << " " << v.z << "\n";
        }

        std::vector<std::array<int, 3>> faces = generateCubeFaces(vertexOffset);

        // tulis face
        for (std::array<int, 3> f : faces)
        {
            file << "f " << f[0] << " " << f[1] << " " << f[2] << "\n";
        }

        vertexOffset += 8;
    }

    file.close();
}