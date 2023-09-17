#include "SpriteBatch.hpp"
#include "Log.h"

namespace DroidBlaster {

    SpriteBatch::SpriteBatch(TimeManager &pTimeManager, Graphics::Manager &pGraphicsManager)
            : m_timeManager(pTimeManager), m_graphicsManager(pGraphicsManager),
              m_sprites(), m_spriteCount(0),
              m_vertices(), m_vertexCount(0),
              m_indexes(), m_indexCount(0),
              m_shaderProgram(0),
              aPosition(-1), aTexture(-1), uProjection(-1), uTexture(-1) {
        m_graphicsManager.registerComponent(this);
    }

    SpriteBatch::~SpriteBatch() {
        for (auto sprite: m_sprites) {
            delete sprite;
        }
    }

    Sprite *
    SpriteBatch::registerSprite(Resource &pTextureResource, int32_t pWidth, int32_t pHeight) {
        int32_t spriteCount = m_spriteCount;
        int32_t index = spriteCount * 4; // Первая вершина

        // Пересчитать содержимое индексного буфера
        GLushort *indexes = (&m_indexes[0]) + spriteCount * 6;
        m_indexes[m_indexCount++] = index + 0;
        m_indexes[m_indexCount++] = index + 1;
        m_indexes[m_indexCount++] = index + 2;
        m_indexes[m_indexCount++] = index + 2;
        m_indexes[m_indexCount++] = index + 1;
        m_indexes[m_indexCount++] = index + 3;

        // Добавить новый спрайт в массив
        m_sprites[m_spriteCount] = new Sprite(m_graphicsManager, pTextureResource, pWidth, pHeight);
        return m_sprites[m_spriteCount++];
    }

    static const char *VERTEX_SHADER =
            "attribute vec4 aPosition;\n"
            "attribute vec2 aTexture;\n"
            "varying vec2 vTexture;\n"
            "uniform mat4 uProjection;\n"
            "void main() {\n"
            "   vTexture = aTexture;\n"
            "   gl_Position = uProjection * aPosition;\n"
            "}";

    static const char *FRAGMENT_SHADER =
            "precision mediump float;\n"
            "varying vec2 vTexture;\n"
            "uniform sampler2D u_texture;\n"
            "void main() {\n"
            "    gl_FragColor = texture2D(u_texture, vTexture);\n"
            "}";

    status SpriteBatch::load() {
        m_shaderProgram = m_graphicsManager.loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
        if (m_shaderProgram == 0) return STATUS_KO;
        aPosition = glGetAttribLocation(m_shaderProgram, "aPosition");
        aTexture = glGetAttribLocation(m_shaderProgram, "aTexture");
        uProjection = glGetUniformLocation(m_shaderProgram, "uProjection");
        uTexture = glGetUniformLocation(m_shaderProgram, "u_texture");

        // Загрузить спрайты
        for (int32_t i = 0; i != m_spriteCount; ++i) {
            if (m_sprites[i]->load(m_graphicsManager) != STATUS_OK) goto ERROR;
        }
        return STATUS_OK;

        ERROR:
        Log::error("Error loading sprite batch");
        return STATUS_KO;
    }

    void SpriteBatch::draw() {
        glUseProgram(m_shaderProgram);
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, m_graphicsManager.getProjectionMatrix());
        glUniform1i(uTexture, 0);
        glEnableVertexAttribArray(aPosition);
        glVertexAttribPointer(
                aPosition, // индекс аттрибута
                2, // размер в байтах (x и y)
                GL_FLOAT, // тип данных
                GL_FALSE, // признак нормализованности
                sizeof(Sprite::Vertex), // шаг
                &(m_vertices[0].x) // место
        );
        glEnableVertexAttribArray(aTexture);
        glVertexAttribPointer(
                aTexture, // индекс аттрибута
                2, // размер в байтах (u и v)
                GL_FLOAT, // тип данных
                GL_FALSE, // признак нормализованности
                sizeof(Sprite::Vertex), // шаг
                &(m_vertices[0].u) // место
        );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const int32_t vertexPerSprite = 4;
        const int32_t indexPerSprite = 6;
        float timeStep = m_timeManager.elapsed();
        int32_t spriteCount = m_spriteCount;
        int32_t currentSprite = 0, firstSprite = 0;
        while (bool canDraw = (currentSprite < spriteCount)) {
            // Выбрать текстуру
            Sprite *sprite = m_sprites[currentSprite];
            GLuint currentTexture = sprite->m_texture;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sprite->m_texture);

            // Сгенерировать вершины спрайтов с текущей текстурой
            do {
                sprite = m_sprites[currentSprite];
                if (sprite->m_texture == currentTexture) {
                    Sprite::Vertex *vertices = (&m_vertices[currentSprite * 4]);
                    sprite->draw(vertices, timeStep);
                } else {
                    break;
                }
            } while (canDraw = (++currentSprite < spriteCount));
            glDrawElements(
                    GL_TRIANGLES,
                    (currentSprite - firstSprite) * indexPerSprite, // число индексов
                    GL_UNSIGNED_SHORT, // тип данных в индексном буфере
                    &m_indexes[firstSprite * indexPerSprite] // первый индекс
            );
            firstSprite = currentSprite;
        }
        glUseProgram(0);
        glDisableVertexAttribArray(aPosition);
        glDisableVertexAttribArray(aTexture);
        glDisable(GL_BLEND);
    }
}