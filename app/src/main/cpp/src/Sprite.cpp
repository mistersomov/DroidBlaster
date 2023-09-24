#include "Sprite.hpp"
#include "Log.h"

namespace DroidBlaster {
    Sprite::Sprite(Graphics::Manager &pGraphicsManager, Resource &pTextureResource,
                   int32_t pWidth, int32_t pHeight) : location(),
                                                      m_textureResource(pTextureResource),
                                                      m_texture(0),
                                                      m_sheetWidth(0), m_sheetHeight(0),
                                                      m_spriteWidth(pWidth),
                                                      m_spriteHeight(pHeight),
                                                      m_frameCount(0), m_frameXCount(0),
                                                      m_frameYCount(0),
                                                      m_animStartFrame(0), m_animFrameCount(1),
                                                      m_animSpeed(0), m_animFrame(0),
                                                      m_animLoop(false) {}

    status Sprite::load(Graphics::Manager &pGraphicsManager) {
        DroidBlaster::Graphics::TextureProperties *textureProperties = pGraphicsManager.loadTexture(
                m_textureResource);
        if (textureProperties == nullptr) return STATUS_KO;
        m_texture = textureProperties->texture;
        m_sheetWidth = textureProperties->width;
        m_sheetHeight = textureProperties->height;
        m_frameXCount = m_sheetWidth / m_spriteWidth;
        m_frameYCount = m_sheetHeight / m_spriteHeight;
        m_frameCount = (m_sheetHeight / m_spriteHeight) * (m_sheetWidth / m_spriteWidth);
        return STATUS_OK;
    }

    void Sprite::setAnimation(int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop) {
        m_animStartFrame = pStartFrame;
        m_animFrame = 0.0f, m_animSpeed = pSpeed, m_animLoop = pLoop;
        m_animFrameCount = pFrameCount;
    }

    void Sprite::draw(Sprite::Vertex *pVertices, float pTimeStep) {
        int32_t currentFrame, currentFrameX, currentFrameY;

        // Обновить анимацию в циклическом режиме
        m_animFrame += pTimeStep * m_animSpeed;
        if (m_animLoop) {
            currentFrame = (m_animStartFrame + int32_t(m_animFrame) % m_animFrameCount);
        } else {
            // Обновить анимацию в однократном режиме
            if (animationEnded()) {
                currentFrame = m_animStartFrame + (m_animFrameCount - 1);
            } else {
                currentFrame = m_animStartFrame + int32_t(m_animFrame);
            }
        }

        // Вычислить индексы X и Y кадра по его порядковому номеру.
        currentFrameX = currentFrame % m_frameXCount;
        // Преобразовать currentFrameY из координат OpenGL
        // в координаты с началом в левом верхнем углу
        currentFrameY = m_frameYCount - 1 - (currentFrame / m_frameXCount);

        // Нарисовать выбранный кадр
        GLfloat posX1 = location.x - float(m_spriteWidth / 2);
        GLfloat posY1 = location.y - float(m_spriteHeight / 2);
        GLfloat posX2 = posX1 + m_spriteWidth;
        GLfloat posY2 = posY1 + m_spriteHeight;

        GLfloat u1 = GLfloat(currentFrameX * m_spriteWidth) / GLfloat(m_sheetWidth);
        GLfloat u2 = GLfloat((currentFrameX + 1) * m_spriteWidth) / GLfloat(m_sheetWidth);
        GLfloat v1 = GLfloat(currentFrameY * m_spriteHeight) / GLfloat(m_sheetHeight);
        GLfloat v2 = GLfloat((currentFrameY + 1) * m_spriteHeight) / GLfloat(m_sheetHeight);

        pVertices[0].x = posX1;
        pVertices[0].y = posY1;
        pVertices[0].u = u1;
        pVertices[0].v = v1;
        pVertices[1].x = posX1;
        pVertices[1].y = posY2;
        pVertices[1].u = u1;
        pVertices[1].v = v2;
        pVertices[2].x = posX2;
        pVertices[2].y = posY1;
        pVertices[2].u = u2;
        pVertices[2].v = v1;
        pVertices[3].x = posX2;
        pVertices[3].y = posY2;
        pVertices[3].u = u2;
        pVertices[3].v = v2;
    }
}