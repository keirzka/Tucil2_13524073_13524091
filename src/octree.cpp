#include "octree.hpp"
#include "geometry.hpp"

OctreeNode::OctreeNode(const AABB &bounds, int depth) : bounds(bounds), isLeaf(false), depth(depth)
{
    for (int i = 0; i < 8; i++)
        children[i] = nullptr;
}

OctreeNode::~OctreeNode()
{
    for (int i = 0; i < 8; i++)
        delete children[i];
}

OctreeNode *buildOctree(
    const std::vector<Triangle> &triangles,
    const AABB &bounds,
    int depth,
    int maxDepth,
    int *nodeCount,
    int *pruneCount);

// memotong node kubus jadi 8 node anak kubus
static std::vector<AABB> subdivide(const AABB &box)
{
    vec3 center = aabb_center(box);
    vec3 min = box.min;
    vec3 max = box.max;

    return {
        {min, center},
        {{center.x, min.y, min.z}, {max.x, center.y, center.z}},
        {{min.x, center.y, min.z}, {center.x, max.y, center.z}},
        {{center.x, center.y, min.z}, {max.x, max.y, center.z}},
        {{min.x, min.y, center.x}, {center.x, center.y, max.z}},
        {{center.x, min.y, center.z}, {max.x, center.y, max.z}},
        {{min.x, center.y, center.z}, {center.x, max.y, max.z}},
        {center, max}};
}

OctreeNode *buildOctree(
    const std::vector<Triangle> &triangles,
    const AABB &bounds,
    int depth,
    int maxDepth,
    int *nodeCount,
    int *pruneCount)
{
    nodeCount[depth]++;

    bool hasIntersection = false;
    for (const auto &tri : triangles)
    {
        if (triangleIntersectsAABB(tri, bounds))
        {
            hasIntersection = true;
            break;
        }
    }

    // node kosong -> pruning
    if (!hasIntersection)
    {
        pruneCount[depth]++;
        return nullptr;
    }

    // bikin octree node baru karena udah pasti nodenya gk kosong
    OctreeNode *node = new OctreeNode(bounds, depth);

    // base rekursi
    if (depth == maxDepth)
    {
        node->isLeaf = true;
        return node;
    }

    // belum mencapai maxDepth -> rekursi
    // divide: membagi jadi 8 anak kubus
    std::vector<AABB> childBounds = subdivide(bounds);
    // conquer: setiap anak diselesaikan secara independen
    // nullptr berarti anak tersebut adalah ruang kosong (pruned)
    for (int i = 0; i < 8; i++)
    {
        node->children[i] = buildOctree(
            triangles,
            childBounds[i],
            depth + 1,
            maxDepth,
            nodeCount,
            pruneCount);
    }
}