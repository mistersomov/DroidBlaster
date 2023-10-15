#ifndef DROIDBLASTER_SPRITEBATCH_HPP
#define DROIDBLASTER_SPRITEBATCH_HPP

#include "manager/GraphicsManager.h"
#include "manager/TimeManager.h"
#include "Sprite.hpp"
#include "Types.h"

#include <GLES2/gl2.h>
#include <vector>

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

        std::vector<Sprite*> m_sprites;
        std::vector<Sprite::Vertex> m_vertices;
        std::vector<GLushort> m_indexes;
        GLuint m_shaderProgram;

        GLuint aPosition, aTexture;
        GLuint uProjection, uTexture;
    };
}

#endif //DROIDBLASTER_SPRITEBATCH_HPP