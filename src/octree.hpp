#pragma once
#include "geometry.hpp"

class OctreeNode
{
public:
    AABB bounds;
    OctreeNode *children[8];
    bool isLeaf;
    int depth;

    OctreeNode(const AABB &bounds, int depth);
    ~OctreeNode();
};

OctreeNode *buildOctree(
    const std::vector<Triangle> &triangles,
    const AABB &bounds,
    int depth,
    int maxDepth,
    int *nodeCount,
    int *pruneCount);