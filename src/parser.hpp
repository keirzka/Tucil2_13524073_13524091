#pragma once
#include "geometry.hpp"
#include <string>
#include <vector>

struct ParsedOBJ
{
    std::vector<Triangle> triangles;
    std::vector<vec3> vertices;
};

bool parseOBJ(const std::string &filepath, ParsedOBJ &result, std::string &erorrMessage);