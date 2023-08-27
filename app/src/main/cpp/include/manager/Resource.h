#pragma once

#include "Types.h"
#include <android_native_app_glue.h>

namespace DroidBlaster {
    class Resource {
    public:
        Resource(android_app* pApplication, const char* pPath);

        status open();
        void close();
        status read(void* pBuffer, size_t pCount);

        bool operator==(const Resource& pOther);

    private:
        const char* m_path;
        AAssetManager* m_assetManager;
        AAsset* m_asset;
    };
}