#pragma once
#include <raylib.h>

namespace RayEngine{
    class AnimatedSprite{
    public:
        AnimatedSprite() {}
        ~AnimatedSprite() {}
        AnimatedSprite(
            const Texture2D &spriteTexture,
            const Vector2 &origin,
            const Vector2 &frameSize,
            const Vector2 &frameOrigin,
            int frameColumns,
            double frameTime,
            int frameCount,
            int firstFrame,
            bool mRepeat
        );
        void Update(double deltaTime);
        void Render(
            const Vector2 &position,
            float rotation,
            const Vector2 &scale,
            bool flipX = false, 
            bool flipY = false
        );
        void Play();
        void Stop();
        void Reset();
        void SetFrame(int frame);
    private:
        Texture2D m_Texture;
        Vector2 m_Origin;
        Vector2 m_FrameSize;
        Vector2 m_FrameOrigin;
        int m_FrameColumns = 1;
        int m_FrameCount = 0;
        int m_FirstFrame = 0;
        double m_FrameTime = 0.0;
        bool m_Repeat;

        double m_EclasedTime = 0.0;
        bool m_Playing = false;
    };
}