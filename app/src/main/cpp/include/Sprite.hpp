#ifndef DROIDBLASTER_SPRITE_HPP
#define DROIDBLASTER_SPRITE_HPP

#include "manager/GraphicsManager.h"
#include "manager/Resource.h"
#include "Types.h"

#include <GLES2/gl2.h>

namespace DroidBlaster {
    class SpriteBatch;

    class Sprite {
        friend class SpriteBatch;

    public:
        struct Vertex {
            GLfloat x, y, u, v;
        };

        Sprite(Graphics::Manager &pGraphicsManager, Resource &pTextureResource,
               int32_t pWidth, int32_t pHeight);

        void setAnimation(int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop);

        bool animationEnded() { return m_animFrame > (m_animFrameCount - 1); };

        Location location;

    protected:
        status load(Graphics::Manager &pGraphicsManager);

        void draw(Vertex pVertices[4], float pTimeStep);

    private:
        Resource &m_textureResource;
        GLuint m_texture;

        // Кадр
        int32_t m_sheetHeight, m_sheetWidth;
        int32_t m_spriteHeight, m_spriteWidth;
        int32_t m_frameXCount, m_frameYCount, m_frameCount;

        // Анимация
        int32_t m_animStartFrame, m_animFrameCount;
        float m_animSpeed, m_animFrame;
        bool m_animLoop;
    };
}

#endif //DROIDBLASTER_SPRITE_HPP