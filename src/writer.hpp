#include "octree.hpp"
#include <array>
#include <fstream>
using namespace std;

void collectLeafNodes(OctreeNode* node, std::vector<OctreeNode*>& leaves);

vector<vec3> generateCubeVertices(const AABB& box);

vector<array<int, 3>> generateCubeFaces(int offset);

void writeOBJ(const std::string& filename, const vector<OctreeNode*>& leaves);

