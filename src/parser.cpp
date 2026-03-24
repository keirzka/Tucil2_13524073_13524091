#include "parser.hpp"
#include <fstream>
#include <sstream>

bool parseOBJ(const std::string &filepath, ParsedOBJ &result, std::string &erorrMessage)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        erorrMessage = "Tidak bisa membuka file " + filepath;
        return false;
    }

    std::vector<vec3> vertices; // vertices sementara
    std::string line;
    int lineNumber = 0; // buat nyari kesalahan format input

    while (std::getline(file, line))
    {
        lineNumber++;
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream ss(line);
        std::string token;
        ss >> token; // baca huruf pertama dalam suatu baris

        if (token == "v")
        {
            vec3 v;
            if (!(ss >> v.x >> v.y >> v.z))
            {
                erorrMessage = "Format vertex tidak valid di baris " + std::to_string(lineNumber);
                return false;
            }
            vertices.push_back(v);
        }

        else if (token == "f")
        {
            int i, j, k;
            if (!(ss >> i >> j >> k))
            {
                erorrMessage = "Format face tidak valid di baris " + std::to_string(lineNumber);
                return false;
            }

            int n = (int)vertices.size();
            if (i < 1 || j < 1 || k < 1 || i > n || j > n || k > n)
            {
                erorrMessage = "Indeks face tidak valid di baris " + std::to_string(lineNumber);
                return false;
            }
            Triangle tri;
            tri.v0 = vertices[i - 1];
            tri.v1 = vertices[j - 1];
            tri.v2 = vertices[k - 1];
            result.triangles.push_back(tri);
        }
    }

    if (vertices.empty())
    {
        erorrMessage = "File tidak mengandung vertex";
        return false;
    }

    if (result.triangles.empty())
    {
        erorrMessage = "File tidak mengandung face";
        return false;
    }

    result.vertices = vertices;
    return true;
}