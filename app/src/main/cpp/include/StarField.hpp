#ifndef DROIDBLASTER_STARFIELD_HPP
#define DROIDBLASTER_STARFIELD_HPP

#include "manager/GraphicsManager.h"
#include "manager/TimeManager.h"
#include "Types.h"

#include <GLES2/gl2.h>

namespace DroidBlaster {
    class StarField : public Graphics::Component {
    public:
        StarField(android_app *pApplication, TimeManager &pTimeManager,
                  Graphics::Manager &pGraphicsManager, int32_t pStarCount,
                  Resource &pTextureResource);

        status load();
        void draw();

    private:
        struct Vertex {
            GLfloat x, y, z;
        };

        TimeManager& m_timeManager;
        Graphics::Manager& m_graphicsManager;

        int32_t m_starCount;
        Resource& m_textureResource;

        GLuint m_vertexBuffer;
        GLuint m_texture;
        GLuint m_shaderProgram;
        GLuint aPosition;
        GLuint uProjection;
        GLuint uTime;
        GLuint uHeight;
        GLuint uTexture;
    };
}

#endif //DROIDBLASTER_STARFIELD_HPP