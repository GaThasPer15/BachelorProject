#pragma once

namespace GTPMath{
    template<typename T>
    class Vec2{

    };
    template<typename T>
    class Vec3{
        public:
            Vec3():x(0), y(0), z(0){}
            Vec3(T x, T y, T z){
                this->x = x;
                this->y = y;
                this->z = z;
            }
            T x;
            T y;
            T z;
    };
}