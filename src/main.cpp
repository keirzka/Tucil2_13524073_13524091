#include <iostream>
#include "parser.hpp"
#include "geometry.hpp"
#include "octree.hpp"
#include "writer.hpp"
#include <chrono>

int main()
{
    // Input file objek
    string fileName;
    std::cout << "Masukkan nama file objek (.obj) pada path ../test/Objek/ : ";
    std::cin >> fileName;
    string filePath = "../test/Objek/" + fileName;

    // validasi keberadaan file input
    ifstream fileCheck(filePath);
    if (!fileCheck.is_open()) {
        cerr << "Error: Tidak ditemukan file " << fileName << endl;
        return 1;
    }
    fileCheck.close();

    // Input maxDepth
    int maxDepth;
    std::cout << "Masukkan maximum depth octree : ";
    std::cin >> maxDepth;


    ParsedOBJ parsed;
    std::string errorMsg;

    if (!parseOBJ(filePath, parsed, errorMsg))
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

    std::vector<int> nodeCount(maxDepth + 1, 0);
    std::vector<int> pruneCount(maxDepth + 1, 0);

    // Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    OctreeNode *root = buildOctree(
        parsed.triangles,
        bounds,
        0,
        maxDepth,
        nodeCount.data(),
        pruneCount.data());
    
    // Collect Leaves
    std::vector<OctreeNode*> leaves;
    collectLeafNodes(root, leaves);

    // End Timer
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();

    // Perhitungan
    int voxelCount = leaves.size();
    int vertexCount = voxelCount * 8;
    int faceCount = voxelCount * 12;

    // Generate Output Objek
    std::string outputPath = filePath;

    // menyesuaikan nama file output
    size_t pos = outputPath.rfind(".obj");
    if (pos != std::string::npos) {
        outputPath = outputPath.substr(0, pos);
    }

    outputPath += "_output.obj";

    writeOBJ(outputPath, leaves);

    // ============ CLI Output ==============
    std::cout << "\n=============== HASIL VOXELIZATION OBJEK ===============\n";
    std::cout << "Jumlah voxel yang terbentuk : " << voxelCount << " voxel" << endl;
    std::cout << "Jumlah vertex yang terbentuk : " << vertexCount << " vertex" <<  endl;
    std::cout << "Jumlah faces yang terbentuk : " << faceCount << " faces" << endl << endl;
    std::cout << "Statistik Node Octree : " << endl;
    for (int i = 0; i <= maxDepth; i++){
        std::cout << "Depth-" << i << " : " << nodeCount[i] << " node" << endl;
    }

    std::cout << endl;

    std::cout << "Statistik Node Octree Pruned : " << endl;
        for (int i = 0; i < maxDepth; i++){
        std::cout << "Depth-" << i << " : " << pruneCount[i] << " node pruned" << endl;
    }

    std::cout << endl;

    std::cout << "Kedalaman octree : " << maxDepth << endl;
    std::cout << "Processing time : " << duration << " seconds" << endl;
    std::cout << "Output objek berhasil disimpan pada file : " << outputPath << endl;
    
    delete root;
    return 0;
}