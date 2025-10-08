#pragma once 

#include <raylib.h>
#include <raymath.h>
#include "../Game/Entity.h"
#include "../MathLib/MathLib.h"

namespace physic{
    enum CollisionShape{
        Circle, Triangle, Rectangle
    };
    struct Data{};
    struct CircleData : public Data{
        public:
            float radius;
    };
    struct PolygonData : public Data{
        public:
            std::vector<Vector2> BasePoints;
            std::vector<Vector2> points;
    };

    class PhysicBody{
        public: 
            PhysicBody() = default;
            PhysicBody(CollisionShape shp, float var1, float val2 = 0);
            PhysicBody(PhysicBody &&otherBody) noexcept;
            PhysicBody& operator=(PhysicBody&& other) noexcept;
            ~PhysicBody();
            
            void Update(double deltaTime);
            bool operator!=(const PhysicBody &otherBody);
            bool CheckCollision(const PhysicBody &otherBody);
            void HandleCollision(PhysicBody &OtherBody);
            void SetVelocity(Vector2 vel = {0.0f, 0.0f}) {velocity = vel;}
            void SetPosition(Vector2 pos) {position = pos;}
            void SetRotation(float r) {rotation = r;}
            void RenderCollider() const;
            CollisionShape GetShape() const {return shape;}

            const std::vector<Vector2> &GetPoints() const;
            float FindMinSeparation(const std::vector<Vector2> pointsA, const std::vector<Vector2> &pointsB);
            bool IsCollidingPolygonPolygon(const std::vector<Vector2> &polyA, const std::vector<Vector2> &pointsB);
            bool IsCollidingPolygonCircle(const std::vector<Vector2> pts, const Vector2 posC, const float r);
            bool IsCollidingCircleCircle(const Vector2 posA, const Vector2 posB, const float rA, const float rB);
            bool SAT(std::vector<Vector2> polyA, std::vector<Vector2> polyB);
        private:
            int ID;
            static int HighestID;
            Vector2 position;
            float rotation;
            Vector2 velocity;
            CollisionShape shape;
            Data *data;

            void ProjectPoly(float &min, float &max, Vector2 axis, std::vector<Vector2> poly);
    };

    
}