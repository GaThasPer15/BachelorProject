#include "AnimatedSprite.h"
#include <algorithm>

RayEngine::AnimatedSprite::AnimatedSprite(
    const Texture2D &spriteTexture,
    const Vector2 &origin,
    const Vector2 &frameSize,
    const Vector2 &frameOrigin,
    int frameColumns,
    double frameTime,
    int frameCount,
    int firstFrame,
    bool mRepeat
):
    m_Texture(spriteTexture),
    m_Origin(origin),
    m_FrameSize(frameSize),
    m_FrameOrigin(frameOrigin),
    m_FrameColumns(frameColumns),
    m_FrameTime(frameTime),
    m_FrameCount(frameCount),
    m_FirstFrame(firstFrame),
    m_Repeat(mRepeat)
{
    m_FrameOrigin.x = std::clamp(m_FrameOrigin.x, 0.0f, 1.0f);
    m_FrameOrigin.x = std::clamp(m_FrameOrigin.y, 0.0f, 1.0f);
}

void RayEngine::AnimatedSprite::Update(double deltaTime){
    if(!m_Playing)
        return;
    m_EclasedTime += deltaTime;
}

void RayEngine::AnimatedSprite::Render(const Vector2 &position, float rotation, const Vector2 &scale, bool flipX, bool flipY){
    int currentFrame = (int) (m_EclasedTime / m_FrameTime);
    if(m_Repeat){
        currentFrame %= m_FrameCount;
    }
    else{
        currentFrame = std::min(currentFrame, m_FrameCount - 1);
    }
    currentFrame += m_FirstFrame;
    const int currentColumn = currentFrame % m_FrameColumns;
    const int currentRow = currentFrame  / m_FrameColumns;
    const Rectangle sourceRect = {
        m_Origin.x + currentColumn * m_FrameSize.x,
        m_Origin.y + currentRow * m_FrameSize.y,
        flipX ? -m_FrameSize.x : m_FrameSize.x,
        flipY ? -m_FrameSize.y : m_FrameSize.y
    };
    const Vector2 spriteSize = {m_FrameSize.x * scale.x, m_FrameSize.y * scale.y};
    const Rectangle destRect  = {
        position.x, position.y, spriteSize.x, spriteSize.y
    };
    const Vector2 origin = {spriteSize.x * m_FrameOrigin.x, spriteSize.y * m_FrameOrigin.y};
    DrawTexturePro(m_Texture, sourceRect, destRect, origin, rotation, WHITE);
}

void RayEngine::AnimatedSprite::Play(){
    m_Playing = true;
}

void RayEngine::AnimatedSprite::Stop(){
    m_Playing = false;
}

void RayEngine::AnimatedSprite::Reset(){
    SetFrame(0);
}
void RayEngine::AnimatedSprite::SetFrame(int frame){
    m_EclasedTime = frame * m_FrameTime;
}