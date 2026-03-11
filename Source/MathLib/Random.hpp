#pragma once 
#include <iostream>

namespace GTPMath{
    namespace Random{
        class HashRNGGenerator{
            uint64_t seed;
            uint32_t channel;
            Vector2 limit;
            bool isLimited;
            uint64_t hash(int32_t x, int32_t y) const{
                uint64_t num = seed;
                num ^= static_cast<uint64_t>(static_cast<uint32_t>(x));
                num *= 0x9e3779b97f4a7c15ULL;
                num ^= static_cast<uint64_t>(static_cast<uint32_t>(y)) << 32;
                num *= 0xbf58476d1ce4e5b9ULL;
                num ^= static_cast<uint64_t>(channel) << 16;
                num *= 0x9e3779b97f4a7c15ULL;
                num ^= num >> 31;
                return num;
            }
        public:
            HashRNGGenerator(uint64_t seed): seed(seed), channel(-1), isLimited(false) {}
            int Int(int32_t x, int32_t y){
                ++channel;
                return static_cast<int>(hash(x, y) & 0xFFFFFFFF);
            }
            float Float(int32_t x, int32_t y);
            Vector2 Vector2(int32_t x, int32_t y){
                ++channel;
                uint64_t num = hash(x, y);
                return ::Vector2{
                    static_cast<float>(num & 0xFFFFFFFF) / static_cast<float>(0xFFFFFFFF),
                    static_cast<float>(num >> 32) / static_cast<float>(0xFFFFFFFF)
                };
            }
            int Int(int32_t x, int32_t y){
                ++channel;
                return static_cast<int>(hash(x, y));
            }
            template <typename T>
            T Next(){
                ++channel;
            }
            void setLimit(float a = 0, float b = 0){
                if(a == 0 && b ==0){
                    isLimited = false;
                }
                else if(a < b){
                    limit.x = a;
                    limit.x = b;
                }
                else std::cerr << "Ivalid arguments in generator limit";
            }
        };

        inline float Rand(int Left, int Right){
            long long a = 60061LL;
            long long c = 12673LL;
            long long m = 1441440LL;
            double temp;
            unsigned long long CPUTime = __rdtsc();
            unsigned long long right = CPUTime >> 32;
            unsigned long long left = CPUTime << 32;
            long long seed = (left & 0x0FFFFFFF00000000) | right;
            std::cout << seed << "\n";
            temp = (a*CPUTime+c)%m;
            temp = temp/m;
            return temp * (Right-Left) + Left;
        }
    }
}