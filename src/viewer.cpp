#include "viewer.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>

static const int MAX_VOXELS_PER_BATCH = 8000;

static const int cornerX[8] = {0, 1, 0, 1, 0, 1, 0, 1};
static const int cornerY[8] = {0, 0, 1, 1, 0, 0, 1, 1};
static const int cornerZ[8] = {0, 0, 0, 0, 1, 1, 1, 1};

static const int faces[12][3] = {
    {0, 1, 2}, {1, 3, 2}, {4, 6, 5}, {5, 6, 7}, {0, 2, 4}, {2, 6, 4}, {1, 5, 3}, {3, 5, 7}, {0, 4, 1}, {1, 4, 5}, {2, 3, 6}, {3, 7, 6}};

static Model buildBatchModel(const std::vector<OctreeNode *> &leaves,
                             int start, int count)
{
    int vertCount = count * 8;
    int triCount = count * 12;

    Mesh mesh = {0};
    mesh.vertexCount = vertCount;
    mesh.triangleCount = triCount;
    mesh.vertices = (float *)MemAlloc(vertCount * 3 * sizeof(float));
    mesh.indices = (unsigned short *)MemAlloc(triCount * 3 * sizeof(unsigned short));

    int vi = 0;
    int ii = 0;

    for (int v = 0; v < count; v++)
    {
        AABB b = leaves[start + v]->bounds;
        float xs[2] = {b.min.x, b.max.x};
        float ys[2] = {b.min.y, b.max.y};
        float zs[2] = {b.min.z, b.max.z};

        for (int c = 0; c < 8; c++)
        {
            mesh.vertices[vi++] = xs[cornerX[c]];
            mesh.vertices[vi++] = ys[cornerY[c]];
            mesh.vertices[vi++] = zs[cornerZ[c]];
        }
        int baseVertex = v * 8;
        for (int f = 0; f < 12; f++)
        {
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][0]);
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][1]);
            mesh.indices[ii++] = (unsigned short)(baseVertex + faces[f][2]);
        }
    }

    UploadMesh(&mesh, false);
    return LoadModelFromMesh(mesh);
}

void runViewer(const std::vector<OctreeNode *> &leaves)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "3D Voxel Viewer");

    // Bangun model dalam beberapa batch
    int total = (int)leaves.size();
    std::vector<Model> models;

    for (int start = 0; start < total; start += MAX_VOXELS_PER_BATCH)
    {
        int count = std::min(MAX_VOXELS_PER_BATCH, total - start);
        models.push_back(buildBatchModel(leaves, start, count));
    }

    // Hitung bounding box seluruh objek untuk posisi kamera
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
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);
    float yaw = 45.0f;
    float pitch = 30.0f;
    float dist = radius * 2.8f;

    while (!WindowShouldClose())
    {
        // Rotasi
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            yaw -= delta.x * 0.3f;
            pitch -= delta.y * 0.3f;
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // Zoom
        float wheel = GetMouseWheelMove();
        dist -= wheel * dist * 0.1f;

        float yawRad = yaw * DEG2RAD;
        float pitchRad = pitch * DEG2RAD;

        camera.position.x = cx + dist * cosf(pitchRad) * sinf(yawRad);
        camera.position.y = cy + dist * sinf(pitchRad);
        camera.position.z = cz + dist * cosf(pitchRad) * cosf(yawRad);
        camera.target = Vector3{cx, cy, cz};

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        // Render semua batch
        for (auto &m : models)
            DrawModel(m, Vector3{0.0f, 0.0f, 0.0f}, 1.0f, LIGHTGRAY);

        // Wireframe per voxel
        for (const auto &leaf : leaves)
        {
            vec3 mn = leaf->bounds.min;
            vec3 mx = leaf->bounds.max;
            float lcx = (mn.x + mx.x) * 0.5f;
            float lcy = (mn.y + mx.y) * 0.5f;
            float lcz = (mn.z + mx.z) * 0.5f;
            DrawCubeWires(Vector3{lcx, lcy, lcz},
                          mx.x - mn.x, mx.y - mn.y, mx.z - mn.z,
                          DARKGRAY);
        }

        DrawGrid(20, 1.0f);
        EndMode3D();

        DrawText("Voxel Viewer", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 35);
        EndDrawing();
    }
    for (auto &m : models)
        UnloadModel(m);
    CloseWindow();
}