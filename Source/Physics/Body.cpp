#include "Body.h"

#include <iostream>
#include <math.h>
#include <algorithm>


int physic::PhysicBody::HighestID = 0;
physic::PhysicBody::PhysicBody(CollisionShape shp, float val1, float val2){
    ID = HighestID + 1;
    HighestID++;
    
    shape = shp;
    if(shape == CollisionShape::Circle){
        data = new CircleData();
        auto *circleData = static_cast<CircleData*>(data);
        circleData->radius = val1;
    }
    else if(shape == CollisionShape::Triangle){
        data = new PolygonData();
        auto *polygonData = static_cast<PolygonData*>(data);
        polygonData->BasePoints.push_back(Vector2{val1/2, val2*0.333f});
        polygonData->BasePoints.push_back(Vector2{-val1/2, val2*0.333f});
        polygonData->BasePoints.push_back(Vector2{0, -val2*0.667f});
    }
    else if(shape == CollisionShape::Rectangle){
        data = new PolygonData();
        auto *polygonData = static_cast<PolygonData*>(data);
        polygonData->BasePoints.push_back(Vector2{val1/2, val2/2});
        polygonData->BasePoints.push_back(Vector2{-val1/2, val2/2});
        polygonData->BasePoints.push_back(Vector2{-val1/2, -val2/2});
        polygonData->BasePoints.push_back(Vector2{val1/2, -val2/2});
    }
}
physic::PhysicBody::~PhysicBody(){
    delete data;
}
physic::PhysicBody::PhysicBody(PhysicBody &&other) noexcept
    : ID(other.ID), position(other.position), velocity(other.velocity), data(other.data), shape(other.shape)
{
    other.ID = -1;
    other.data = nullptr;
}
physic::PhysicBody& physic::PhysicBody::operator=(physic::PhysicBody&& other) noexcept {
    if(this != &other){
        delete data;
        ID = other.ID;
        position = other.position;
        velocity = other.velocity;
        shape = other.shape;
        data = other.data;
        other.ID = -1;
        other.data = nullptr;
    }
    return *this;
}
bool physic::PhysicBody::operator!=(const physic::PhysicBody &OtherBody){
    if(ID == OtherBody.ID)
        return false;
    return true;
}
const std::vector<Vector2> &physic::PhysicBody::GetPoints() const{
    if(shape != CollisionShape::Circle){
        auto polygonData = static_cast<PolygonData*>(data);
        return polygonData->points;
    }
    static std::vector<Vector2> emptyPoints;
    return emptyPoints;
}

void physic::PhysicBody::Update(double deltaTime){
    if(shape != CollisionShape::Circle){
        auto *polygonData = static_cast<PolygonData*>(data);
        polygonData->points = polygonData->BasePoints;
        float radRot = rotation * DEG2RAD;
        for(auto &point : polygonData->points){
            point = Vector2Rotate(point, radRot);
            point = Vector2Add(point, position);
        }
    }
}

void physic::PhysicBody::HandleCollision(PhysicBody &otherBody){
    std::cout << "Collision detected!\n";
}

bool physic::PhysicBody::CheckCollision(const PhysicBody &otherBody){
    CollisionShape otherShape = otherBody.GetShape();
    if(otherShape == CollisionShape::Circle && shape == CollisionShape::Circle){
        return IsCollidingCircleCircle(position, otherBody.position, rotation, otherBody.rotation);
    }
    else if(shape == CollisionShape::Circle){
        auto *polygonData = static_cast<PolygonData*>(otherBody.data);
        auto *circleData = static_cast<CircleData*>(data);
        return IsCollidingPolygonCircle(polygonData->points, position, circleData->radius);
    }
    else if(otherShape == CollisionShape::Circle){
        auto *polygonData = static_cast<PolygonData*>(data);
        auto circleData = static_cast<CircleData*>(otherBody.data);
        return IsCollidingPolygonCircle(polygonData->points, otherBody.position, circleData->radius);
    }
    else{
        auto *polygonData = static_cast<PolygonData*>(data);
        return IsCollidingPolygonPolygon(polygonData->points, otherBody.GetPoints());
    }
}

bool physic::PhysicBody::IsCollidingPolygonPolygon(const std::vector<Vector2> &polyA, const std::vector<Vector2> &polyB){
    return SAT(polyA, polyB);
}
bool physic::PhysicBody::SAT(std::vector<Vector2> polyA, std::vector<Vector2> polyB){
    std::vector<std::vector<Vector2>> polys;
    polys.push_back(polyA);
    polys.push_back(polyB);
    for(auto poly : polys){
        for(int i=0; i<poly.size(); i++){
            Vector2 edge = GTPMath::MakeEdgeVector(poly.at(i), poly.at((i+1)%poly.size()));
            Vector2 axis = GTPMath::Vector2Perpendicular(edge);
            axis = Vector2Normalize(axis);
            float minA = Vector2DotProduct(axis, polyA.at(0)), maxA = minA;
            float minB = Vector2DotProduct(axis, polyB.at(0)), maxB = minB;
            ProjectPoly(minA, maxA, axis, polyA);
            ProjectPoly(minB, maxB, axis, polyB);
            if(maxA < minB || maxB < minA)
                return false;
        }
    }
    return true;
}
void physic::PhysicBody::ProjectPoly(float &min, float &max, Vector2 axis, std::vector<Vector2> poly){
    for(auto point: poly){
        float proj = Vector2DotProduct(axis, point);
        if(proj < min)
            min = proj;
        if(proj > max)
            max = proj;
    }
}

bool physic::PhysicBody::IsCollidingCircleCircle(const Vector2 posA, const Vector2 posB, const float rA, const float rB){
    float pit = (posB.x-posA.x)*(posB.x-posA.x) + (posB.y-posA.y)*(posB.y-posA.y);
    pit = sqrt(pit);
    return pit < rA + rB;
}

void physic::PhysicBody::RenderCollider() const{
    if(shape == CollisionShape::Circle){
        auto *circleData = static_cast<CircleData*>(data);
        DrawRing(position, circleData->radius - 0.75f, circleData->radius + 0.75f, 0, 360, 36, RED);
    }
    else{
        auto *polygonData = static_cast<PolygonData*>(data);
        for(int i=0; i<polygonData->points.size(); i++){
            Vector2 va = polygonData->points.at(i);
            Vector2 vb = polygonData->points.at((i+1)%polygonData->points.size());
            DrawLine(va.x, va.y, vb.x, vb.y, RED);
        }
    }
}

bool physic::PhysicBody::IsCollidingPolygonCircle(const std::vector<Vector2> pts, Vector2 circlePosition, float radius){
    
    return false;
}
