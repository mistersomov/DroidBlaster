#include "manager/Resource.h"
#include "sys/stat.h"

namespace DroidBlaster {
    Resource::Resource(android_app *pApplication, const char *pPath) : m_path(pPath), m_assetManager(
            pApplication->activity->assetManager), m_asset(
            nullptr) {}

    status Resource::open() {
        m_asset = AAssetManager_open(m_assetManager, m_path, AASSET_MODE_UNKNOWN);
        return (m_asset != nullptr) ? STATUS_OK : STATUS_KO;
    }

    void Resource::close() {
        if (m_asset != nullptr) {
            AAsset_close(m_asset);
            m_asset = nullptr;
        }
    }

    status Resource::read(void *pBuffer, size_t pCount) {
        int32_t readCount = AAsset_read(m_asset, pBuffer, pCount);
        return (readCount == pCount) ? STATUS_OK : STATUS_KO;
    }

    bool Resource::operator==(const Resource &pOther) {
        return not strcmp(m_path, pOther.m_path);
    }
}