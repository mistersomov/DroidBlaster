#include "StarField.hpp"
#include "Log.h"

namespace DroidBlaster {

    StarField::StarField(android_app *pApplication, TimeManager &pTimeManager,
                         Graphics::Manager &pGraphicsManager, int32_t starCount,
                         Resource &pTextureResource) : m_timeManager(pTimeManager),
                                                       m_graphicsManager(pGraphicsManager),
                                                       m_starCount(starCount),
                                                       m_textureResource(pTextureResource),
                                                       m_vertexBuffer(0),
                                                       m_texture(-1),
                                                       m_shaderProgram(0),
                                                       aPosition(-1),
                                                       uProjection(-1),
                                                       uHeight(-1),
                                                       uTexture(-1),
                                                       uTime(-1) {
        m_graphicsManager.registerComponent(this);
    }

    static const char *VERTEX_SHADER =
            "attribute vec4 aPosition;\n"
            "uniform mat4 uProjection;\n"
            "uniform float uHeight;\n"
            "uniform float uTime;\n"
            "void main() {\n"
            "    const float speed = -800.0;\n"
            "    const float size = 8.0;\n"
            "    vec4 position = aPosition;\n"
            "    position.x = aPosition.x;\n"
            "    position.y = mod(aPosition.y + (uTime * speed * aPosition.z), uHeight);\n"
            "    position.z = 0.0;\n"
            "    gl_Position = uProjection * position;\n"
            "    gl_PointSize = aPosition.z * size;\n"
            "}";

    static const char *FRAGMENT_SHADER =
            "precision mediump float;\n"
            "uniform sampler2D uTexture;\n"
            "void main() {\n"
            "    gl_FragColor = texture2D(uTexture, gl_PointCoord);\n"
            "}";

    status StarField::load() {
        Log::info("Loading star field.");
        Graphics::TextureProperties *textureProperties;

        // Выделить память для временного буфера и заполнить
        // его данными: 1 вершине соответствует 3 вещественных числа (X/Y/Z)
        auto *vertexBuffer = new Vertex[m_starCount];
        for (int32_t i = 0; i != m_starCount; ++i) {
            vertexBuffer[i].x = RAND(m_graphicsManager.getRenderWidth());
            vertexBuffer[i].y = RAND(m_graphicsManager.getRenderHeight());
            vertexBuffer[i].z = RAND(1.0f);
        }

        // Загрузить буфер с вершинами в OpenGL
        m_vertexBuffer = m_graphicsManager.loadVertexBuffer((uint8_t *) vertexBuffer,
                                                            m_starCount * sizeof(Vertex));
        delete[] vertexBuffer;
        if (m_vertexBuffer == 0) goto ERROR;

        // Загрузить текстуру
        textureProperties = m_graphicsManager.loadTexture(m_textureResource);
        if (textureProperties == nullptr) goto ERROR;
        m_texture = textureProperties->texture;

        // Создать и извлечь атрибуты и uniform-переменные шейдера
        m_shaderProgram = m_graphicsManager.loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
        if (m_shaderProgram == 0) goto ERROR;
        aPosition = glGetAttribLocation(m_shaderProgram, "aPosition");
        uProjection = glGetUniformLocation(m_shaderProgram, "uProjection");
        uHeight = glGetUniformLocation(m_shaderProgram, "uHeight");
        uTime = glGetUniformLocation(m_shaderProgram, "uTime");
        uTexture = glGetUniformLocation(m_shaderProgram, "uTexture");
        return STATUS_OK;

        ERROR:
        Log::error("Error loading starfield");
        return STATUS_KO;
    }

    void StarField::draw() {
        glDisable(GL_BLEND);

        // Выбрать буфер с вершинами и определить его организацию
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glEnableVertexAttribArray(aPosition);
        glVertexAttribPointer(
                aPosition, // индекс аттрибута
                3, // число компонентов
                GL_FLOAT, // тип данных
                GL_FALSE, // признак нормализации
                3 * sizeof(GLfloat), // шаг
                (GLvoid *) 0 // первая вершина
        );

        // Выбрать текстуру
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // Выбрать шейдер и передать ему параметры
        glUseProgram(m_shaderProgram);
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, m_graphicsManager.getProjectionMatrix());
        glUniform1f(uHeight, m_graphicsManager.getRenderHeight());
        glUniform1f(uTime, m_timeManager.elapsedTotal());
        glUniform1i(uTexture, 0);

        // Отобразить звёзды
        glDrawArrays(GL_POINTS, 0, m_starCount);

        // Восстановить состояние устройства
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }
}