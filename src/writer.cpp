#include "writer.hpp"

using namespace std;

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
vector<vec3> generateCubeVertices(const AABB& box) {
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
vector<array<int, 3>> generateCubeFaces(int offset) {
    return {
        {offset+1, offset+2, offset+3}, {offset+2, offset+4, offset+3},
        {offset+5, offset+6, offset+7}, {offset+6, offset+8, offset+7},
        {offset+1, offset+2, offset+5}, {offset+2, offset+6, offset+5},
        {offset+2, offset+4, offset+6}, {offset+4, offset+8, offset+6},
        {offset+4, offset+3, offset+8}, {offset+3, offset+7, offset+8},
        {offset+3, offset+1, offset+7}, {offset+1, offset+5, offset+7}
    };
}

// Write to file objek
void writeOBJ(const std::string& filename, const vector<OctreeNode*>& leaves) {
    ofstream file(filename);

    int vertexOffset = 0;

    for (OctreeNode* node : leaves) {

        vector<vec3> vertices = generateCubeVertices(node->bounds);

        // tulis vertex
        for (vec3 v : vertices) {
            file << "v " << v.x << " " << v.y << " " << v.z << "\n";
        }

        vector<array<int, 3>> faces = generateCubeFaces(vertexOffset);

        // tulis face
        for (array<int, 3> f : faces) {
            file << "f " << f[0] << " " << f[1] << " " << f[2] << "\n";
        }

        vertexOffset += 8;
    }

    file.close();
}