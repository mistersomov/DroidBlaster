#ifndef DROIDBLASTER_SPRITEBATCH_HPP
#define DROIDBLASTER_SPRITEBATCH_HPP

#include "manager/GraphicsManager.h"
#include "manager/TimeManager.h"
#include "Sprite.hpp"
#include "Types.h"

#include <GLES2/gl2.h>

namespace DroidBlaster {
    class SpriteBatch : public Graphics::Component {
    public:
        SpriteBatch(TimeManager &pTimeManager, Graphics::Manager &pGraphicsManager);

        ~SpriteBatch();

        Sprite *registerSprite(Resource &pTextureResource, int32_t pWidth, int32_t pHeight);
        status load();
        void draw();

    private:
        TimeManager &m_timeManager;
        Graphics::Manager &m_graphicsManager;

        Sprite *m_sprites[1024];
        int32_t m_spriteCount;
        Sprite::Vertex m_vertices[1024];
        int32_t m_vertexCount;
        GLushort m_indexes[1024];
        int32_t m_indexCount;
        GLuint m_shaderProgram;
        GLuint aPosition, aTexture;
        GLuint uProjection, uTexture;
    };
}

#endif //DROIDBLASTER_SPRITEBATCH_HPP