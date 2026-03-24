#include <iostream>
#include "parser.hpp"
#include "geometry.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: voxelizer <file.obj> <max_depth>\n";
        return 1;
    }

    std::string filepath = argv[1];
    int maxDepth = std::stoi(argv[2]);

    ParsedOBJ parsed;
    std::string errorMsg;

    if (!parseOBJ(filepath, parsed, errorMsg))
    {
        std::cerr << "Error: " << errorMsg << "\n";
        return 1;
    }

    std::cout << "Vertices  : " << parsed.vertices.size() << "\n";
    std::cout << "Triangles : " << parsed.triangles.size() << "\n";

    AABB bounds = computeBounds(parsed.vertices);
    std::cout << "AABB min  : ("
              << bounds.min.x << ", "
              << bounds.min.y << ", "
              << bounds.min.z << ")\n";
    std::cout << "AABB max  : ("
              << bounds.max.x << ", "
              << bounds.max.y << ", "
              << bounds.max.z << ")\n";

    float sideX = bounds.max.x - bounds.min.x;
    float sideY = bounds.max.y - bounds.min.y;
    float sideZ = bounds.max.z - bounds.min.z;
    std::cout << "Cube side : " << sideX
              << " (should equal " << sideY
              << " and " << sideZ << ")\n";

    return 0;
}