#include "manager/GraphicsManager.h"
#include "Log.h"

namespace DroidBlaster {
    using namespace Graphics;

    Manager::Manager(android_app *pApplication) : m_application(pApplication),
                                                  m_renderWidth(0), m_renderHeight(0),
                                                  //m_elements(),
                                                  m_elementCount(0),
                                                  m_display(EGL_NO_DISPLAY),
                                                  m_surface(EGL_NO_SURFACE),
                                                  m_context(EGL_NO_CONTEXT),
                                                  m_textures(), m_textureCount(0),
                                                  m_projectionMatrix(),
                                                  m_shaders(), m_shaderCount(0),
                                                  m_components(), m_componentCount(0) {
        Log::info("Creating GraphicsManager");
    }

    Manager::~Manager() {
        Log::info("Destroying GraphicsManager");

//        for (int32_t i = 0; i != m_elementCount; ++i) {
//            delete m_elements[i];
//        }
    }

    DroidBlaster::status Manager::start() {
        Log::info("Starting GraphicsManager");

        EGLint format, numConfigs, errorResult;
        GLenum status;
        EGLConfig config;

        // Определить требования к экрану. Здесь используется 16-битный режим
        const EGLint DISPLAY_ATTRIBS[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        // Запросить контекст OpenGL ES2
        const EGLint CONTEXT_ATTRIBS[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) goto ERROR;
        if (not eglInitialize(m_display, nullptr, nullptr)) goto ERROR;
        if (not eglChooseConfig(m_display, DISPLAY_ATTRIBS, &config, 1, &numConfigs)
            || numConfigs <= 0) {
            goto ERROR;
        }
        if (not eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format)) goto ERROR;
        ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, format);

        m_surface = eglCreateWindowSurface(m_display, config, m_application->window, nullptr);
        if (m_surface == EGL_NO_SURFACE) goto ERROR;

        m_context = eglCreateContext(m_display, config, nullptr, CONTEXT_ATTRIBS);
        if (m_context == EGL_NO_CONTEXT) goto ERROR;

        if (not eglMakeCurrent(m_display, m_surface, m_surface, m_context)
            || not eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_renderWidth)
            || not eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_renderHeight)
            || m_renderWidth <= 0
            || m_renderHeight <= 0) {
            goto ERROR;
        }
        glViewport(0, 0, m_renderWidth, m_renderHeight);
        glDisable(GL_DEPTH_TEST);

//        //Установить 32-битный формат
//        ANativeWindow_Buffer windowBuffer;
//        if (ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, WINDOW_FORMAT_RGBA_8888) <
//            0) {
//            Log::error("Error while setting buffer geometry");
//            return STATUS_KO;
//        }
//        // Заблокировать буфер окна, чтобы получить его свойства
//        if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) >= 0) {
//            m_renderWidth = windowBuffer.width;
//            m_renderHeight = windowBuffer.height;
//            ANativeWindow_unlockAndPost(m_application->window);
//        } else {
//            Log::error("Error while locking window");
//            return STATUS_KO;
//        }

        // Подготовить матрицу проекции с размерами отображемого окна
        std::memset(m_projectionMatrix[0], 0, sizeof(m_projectionMatrix));
        m_projectionMatrix[0][0] = 2.0f / GLfloat(m_renderWidth);
        m_projectionMatrix[1][1] = 2.0f / GLfloat(m_renderHeight);
        m_projectionMatrix[2][2] = -1.0f;
        m_projectionMatrix[3][0] = -1.0f;
        m_projectionMatrix[3][1] = -1.0f;
        m_projectionMatrix[3][2] = 0.0f;
        m_projectionMatrix[3][3] = 1.0f;

        // Загрузить графические компоненты
        for (int32_t i = 0; i != m_componentCount; ++i) {
            if (m_components[i]->load() != STATUS_OK) return STATUS_KO;
        }

        return STATUS_OK;

        ERROR:
        Log::error("Error while starting GraphicsManager");
        stop();
        return STATUS_KO;
    }

    DroidBlaster::status Manager::update() {
//        static float clearColor = 0.0f;
//        clearColor += 0.001f;
//        glClearColor(clearColor, clearColor, clearColor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int32_t i = 0; i < m_componentCount; ++i) {
            m_components[i]->draw();
        }

        if (eglSwapBuffers(m_display, m_surface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers", eglGetError());
            return STATUS_KO;
        } else {
            return STATUS_OK;
        }
//        // Заблокировать буфер экрана перед рисованием
//        ANativeWindow_Buffer windowBuffer;
//        if (ANativeWindow_lock(m_application->window, &windowBuffer, nullptr) < 0) {
//            Log::error("Error while starting GraphicsManager");
//            return STATUS_KO;
//        }
//        // Очистить окно
//        //std::memset(windowBuffer.bits, 0, windowBuffer.stride * windowBuffer.height * sizeof(uint32_t*));
//        // Вывести графические элементы
//        int32_t maxX = windowBuffer.width - 1;
//        int32_t maxY = windowBuffer.height - 1;
//        for (int32_t i = 0; i != m_elementCount; ++i) {
//            Element *pElement = m_elements[i];
//
//            // Вычислить координаты
//            auto leftX = static_cast<int32_t>(pElement->location.x - pElement->width / 2);
//            auto rightX = static_cast<int32_t>(pElement->location.x + pElement->width / 2);
//            auto leftY = static_cast<int32_t>(windowBuffer.height - pElement->location.y -
//                                              pElement->height / 2);
//            auto rightY = static_cast<int32_t>(windowBuffer.height - pElement->location.y +
//                                               pElement->height / 2);
//
//            // Обрезать координаты
//            if (rightX < 0 || leftX > maxX || rightY < 0 || leftY > maxY) continue;
//            if (leftX < 0) {
//                leftX = 0;
//            } else if (rightX > maxX) {
//                rightX = maxX;
//            }
//            if (leftY < 0) {
//                leftY = 0;
//            } else if (rightY > maxY) {
//                rightY = maxY;
//            }
//            // Нарисовать прямоугольник
//            uint32_t *line = (uint32_t *) (windowBuffer.bits) + (windowBuffer.stride * leftY);
//            for (int iY = leftY; iY != rightY; iY++) {
//                for (int iX = leftX; iX != rightX; iX++) {
//                    line[iX] = 0X000000FF;
//                }
//                line = line + windowBuffer.stride;
//            }
//        }
//        ANativeWindow_unlockAndPost(m_application->window);
//        return STATUS_OK;
    }

//    Element *Manager::registerElement(int32_t pHeight, int32_t pWidth) {
//        m_elements[m_elementCount] = new Element(pWidth, pHeight);
//        return m_elements[m_elementCount++];
//    }

    void Manager::stop() {
        Log::info("Stopping GraphicsManager");

        for (int32_t i = 0; i != m_textureCount; ++i) {
            glDeleteTextures(1, reinterpret_cast<const GLuint *>(&m_textures[i].texture));
        }
        m_textureCount = 0;

        for (auto shader : m_shaders) {
            glDeleteProgram(shader);
        }
        m_shaderCount = 0;

        // Уничтожить контекст OpenGl
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (m_context != EGL_NO_CONTEXT) {
                eglDestroyContext(m_display, m_context);
                m_context = EGL_NO_CONTEXT;
            }
            if (m_surface != EGL_NO_SURFACE) {
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }
            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
    }

    TextureProperties *Manager::loadTexture(Resource &pResource) {
        for (int32_t i = 0; i != m_textureCount; ++i) {
            if (pResource == *m_textures[i].textureResource) {
                Log::info("Found %s in cache", pResource.getPath());
                return &m_textures[i];
            }
        }

        Log::info("Loading texture %s", pResource.getPath());
        TextureProperties *textureProperties;
        GLuint texture;
        GLint format;

        png_byte header[8];
        png_structp pngPtr = nullptr;
        png_infop infoPtr = nullptr;
        png_byte *image = nullptr;
        png_bytep *rowPtrs = nullptr;
        png_int_32 rowSize;
        bool transparency;

        if (pResource.open() != STATUS_OK) goto ERROR;

        Log::info("Checking signature");
        if (pResource.read(header, sizeof(header)) != STATUS_OK) goto ERROR;
        if (png_sig_cmp(header, 0, 8) != 0) goto ERROR;

        Log::info("Creating required structures");
        pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (not pngPtr) goto ERROR;

        infoPtr = png_create_info_struct(pngPtr);
        if (not infoPtr) goto ERROR;

        // Подготовить операцию чтения, передав функцию обратного вызова
        png_set_read_fn(pngPtr, &pResource, callback_readPng);
        // Если во время чтения возникает ошибка, код вернется обратно,
        // сюда, и выполнит переход к метке ERROR
        if (setjmp(png_jmpbuf(pngPtr))) goto ERROR;

        // Игнорировать првые 8 байт
        png_set_sig_bytes(pngPtr, 8);

        // Получить информацию об изображении PNG и обновить структуру PNG
        png_read_info(pngPtr, infoPtr);
        png_int_32 depth, colorType;
        png_uint_32 width, height;
        png_get_IHDR(pngPtr, infoPtr, &width, &height, &depth, &colorType, nullptr, nullptr,
                     nullptr);

        // Создать полноценный альфа-канал, если прозрачность кодируется как массив записей палитры
        // или как единственный цвет прозрачности
        transparency = false;
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(pngPtr);
            transparency = true;
        }

        // Расширить PNG-изображение до 8 бит на канал, если отводится менее 8 бит
        if (depth < 8) {
            png_set_packing(pngPtr);
            // Сжать PNG-изображение с 16 битами на канал до 8 бит
        } else if (depth == 16) {
            png_set_strip_16(pngPtr);
        }

        // Преобразовать изображение в формат RGBA, если необходимо
        switch (colorType) {
            case PNG_COLOR_TYPE_PALETTE:
                png_set_palette_to_rgb(pngPtr);
                format = transparency ? GL_RGBA : GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGB:
                format = transparency ? GL_RGBA : GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGBA:
                format = GL_RGBA;
                break;
            case PNG_COLOR_TYPE_GRAY:
                png_set_expand_gray_1_2_4_to_8(pngPtr);
                format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
                break;
            case PNG_COLOR_TYPE_GA:
                png_set_expand_gray_1_2_4_to_8(pngPtr);
                format = GL_LUMINANCE_ALPHA;
                break;
            default:
                break;
        }

        // Подтвердить все преобразования
        png_read_update_info(pngPtr, infoPtr);

        // Получить размер строки в байтах
        rowSize = static_cast<int32_t >(png_get_rowbytes(pngPtr, infoPtr));
        if (rowSize <= 0) goto ERROR;

        // Выделить буфер для изображения и последней передачи в OpenGl
        image = new png_byte[rowSize * height];

        /**
         * Указатели на строки в буфере с изображением.
         * Строки располагаются в обратном порядке, потому что
         * библиотека OpenGL использует иную систему координат (начало находится в левом нижнем углу), отличающуюся
         * от системы координат PNG (начало - в левом верхнем углу)
        **/
        rowPtrs = new png_bytep[height];

        for (int32_t i = 0; i != height; ++i) {
            rowPtrs[height - (i + 1)] = image + i * rowSize;
        }

        // Прочитать изображение
        png_read_image(pngPtr, rowPtrs);

        // Освободить память и ресурсы
        pResource.close();
        png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
        delete[] rowPtrs;

        GLenum errorResult;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Настроить свойства текстуры
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Загрузить изображение в текстуру OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

        // Завершить работу с текстурой
        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] image;
        if (glGetError() != GL_NO_ERROR) goto ERROR;
        Log::info("Texture size: %d x %d", width, height);

        // Кешировать загруженную текстуру
        textureProperties = &m_textures[m_textureCount++];
        textureProperties->texture = texture;
        textureProperties->textureResource = &pResource;
        textureProperties->width = width;
        textureProperties->height = height;

        return textureProperties;

        ERROR:
        Log::error("Error loading texture into OpenGl");
        pResource.close();
        delete[] rowPtrs, image;
        if (pngPtr != nullptr) {
            png_infop *infoPtrP = infoPtr != nullptr ? &infoPtr : nullptr;
            png_destroy_read_struct(&pngPtr, infoPtrP, nullptr);
        }
        return nullptr;
    }

    void Manager::callback_readPng(png_structp pStruct, png_bytep pData, png_size_t pSize) {
        Resource *pResource = ((Resource *) png_get_io_ptr(pStruct));
        if (pResource->read(pData, pSize) != STATUS_OK) {
            pResource->close();
        }
    }

    void Manager::registerComponent(Component *pComponent) {
        m_components[m_componentCount++] = pComponent;
    }

    GLuint Manager::loadShader(const char *pVertexShader, const char *pFragmentShader) {
        GLint result;
        char log[256];
        GLuint vertexShader, fragmentShader, shaderProgram;

        // Собрать вершинный шейдер
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &pVertexShader, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderInfoLog(vertexShader, sizeof(log), 0, log);
            Log::error("Vertex shader error: %s", log);
            goto ERROR;
        }

        // Собрать фрагментный шейдер
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &pFragmentShader, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderInfoLog(fragmentShader, sizeof(log), 0, log);
            Log::error("Fragment shader error: %s", log);
            goto ERROR;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderInfoLog(shaderProgram, sizeof(log), 0, log);
            Log::error("Shader program error: %s", log);
            goto ERROR;
        }

        m_shaders[m_shaderCount++] = shaderProgram;
        return shaderProgram;

        ERROR:
        Log::error("Error loading shader");
        if (vertexShader > 0) glDeleteShader(vertexShader);
        if (fragmentShader > 0) glDeleteShader(fragmentShader);
        return 0;
    }
}