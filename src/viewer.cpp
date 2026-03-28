#include "viewer.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>

void runViewer(const std::vector<OctreeNode *> &leaves)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "3D Voxel Viewer");

    // Bangun mesh langsung dari leaf nodes
    int voxelCount = (int)leaves.size();
    int vertCount = voxelCount * 8;
    int triCount = voxelCount * 12;

    Mesh mesh = {0};
    mesh.vertexCount = vertCount;
    mesh.triangleCount = triCount;
    mesh.vertices = (float *)MemAlloc(vertCount * 3 * sizeof(float));
    mesh.indices = (unsigned short *)MemAlloc(triCount * 3 * sizeof(unsigned short));

    int cornerX[8] = {0, 1, 0, 1, 0, 1, 0, 1};
    int cornerY[8] = {0, 0, 1, 1, 0, 0, 1, 1};
    int cornerZ[8] = {0, 0, 0, 0, 1, 1, 1, 1};

    int faces[12][3] = {
        {0, 1, 2}, {1, 3, 2}, {4, 6, 5}, {5, 6, 7}, {0, 2, 4}, {2, 6, 4}, {1, 5, 3}, {3, 5, 7}, {0, 4, 1}, {1, 4, 5}, {2, 3, 6}, {3, 7, 6}};

    int vi = 0;
    int ii = 0;

    for (int v = 0; v < voxelCount; v++)
    {
        AABB b = leaves[v]->bounds;
        float xs[2] = {b.min.x, b.max.x};
        float ys[2] = {b.min.y, b.max.y};
        float zs[2] = {b.min.z, b.max.z};
        int baseVertex = v * 8;

        for (int c = 0; c < 8; c++)
        {
            mesh.vertices[vi++] = xs[cornerX[c]];
            mesh.vertices[vi++] = ys[cornerY[c]];
            mesh.vertices[vi++] = zs[cornerZ[c]];
        }

        for (int f = 0; f < 12; f++)
        {
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][0]);
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][1]);
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][2]);
        }
    }

    UploadMesh(&mesh, false);
    Model model = LoadModelFromMesh(mesh);

    // Hitung center dan radius dari seluruh objek
    vec3 totalMin = leaves[0]->bounds.min;
    vec3 totalMax = leaves[0]->bounds.max;

    for (const auto &leaf : leaves)
    {
        totalMin.x = std::min(totalMin.x, leaf->bounds.min.x);
        totalMin.y = std::min(totalMin.y, leaf->bounds.min.y);
        totalMin.z = std::min(totalMin.z, leaf->bounds.min.z);
        totalMax.x = std::max(totalMax.x, leaf->bounds.max.x);
        totalMax.y = std::max(totalMax.y, leaf->bounds.max.y);
        totalMax.z = std::max(totalMax.z, leaf->bounds.max.z);
    }

    float cx = (totalMin.x + totalMax.x) * 0.5f;
    float cy = (totalMin.y + totalMax.y) * 0.5f;
    float cz = (totalMin.z + totalMax.z) * 0.5f;

    float dx = totalMax.x - totalMin.x;
    float dy = totalMax.y - totalMin.y;
    float dz = totalMax.z - totalMin.z;
    float radius = std::sqrt(dx * dx + dy * dy + dz * dz) * 0.5f;

    Camera camera = {0};
    camera.position = Vector3{cx + radius * 2, cy + radius, cz + radius * 2};
    camera.target = Vector3{cx, cy, cz};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        DrawModel(model, Vector3{0.0f, 0.0f, 0.0f}, 1.0f, LIGHTGRAY);

        for (const auto &leaf : leaves)
        {
            vec3 mn = leaf->bounds.min;
            vec3 mx = leaf->bounds.max;
            float lcx = (mn.x + mx.x) * 0.5f;
            float lcy = (mn.y + mx.y) * 0.5f;
            float lcz = (mn.z + mx.z) * 0.5f;
            float sx = mx.x - mn.x;
            float sy = mx.y - mn.y;
            float sz = mx.z - mn.z;
            DrawCubeWires(Vector3{lcx, lcy, lcz}, sx, sy, sz, DARKGRAY);
        }

        DrawGrid(20, 1.0f);
        EndMode3D();
        DrawText("Voxel Viewer", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 35);
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
}