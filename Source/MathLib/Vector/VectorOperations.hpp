#pragma once

#include <raylib.h>

namespace GTPMath{
    inline Vector2 Vector2Perpendicular(Vector2 v){
        return Vector2{-v.y, v.x};
    }
    inline Vector2 Vector2PerpendicularReverse(Vector2 v){
        return Vector2{v.y, -v.x};
    }
    inline Vector2 MakeEdgeVector(Vector2 ver1, Vector2 ver2){
        return {ver2.x - ver1.x, ver2.y - ver1.y};
    }
    inline Vector2 Vector2Add(Vector2 vec1, Vector2 vec2){
        return {vec1.x + vec2.x, vec1.y + vec2.y};
    }
}