#pragma once
#include "octree.hpp"
#include <array>
#include <fstream>

void collectLeafNodes(OctreeNode* node, std::vector<OctreeNode*>& leaves);

std::vector<vec3> generateCubeVertices(const AABB &box);

std::vector<std::array<int, 3>> generateCubeFaces(int offset);

void writeOBJ(const std::string &filename, const std::vector<OctreeNode *> &leaves);
