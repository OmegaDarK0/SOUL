#include "asset.hpp"
namespace soul {
    void asset_manager::init() {
        for (uint32 i = 1; i < MAX_ASSETS; ++i) {
            m_free_indices[MAX_ASSETS - 1 - i] = i;
            m_assets[i].generation = 1;
            m_assets[i].ref_count = 0;
            m_assets[i].texture = nullptr;
            m_assets[i].name[0] = '\0';
        }
        m_free_count = MAX_ASSETS - 1;
    }
    void asset_manager::exit() {
        for (uint32 i = 1; i < MAX_ASSETS; ++i) {
            if (m_assets[i].ref_count > 0 && m_assets[i].texture) {
                void_texture_destroy(m_assets[i].texture);
                m_assets[i].ref_count = 0;
                m_assets[i].texture = nullptr;
            }
        }
    }
    asset asset_manager::load_texture(const VoidWindow* window, const char* name) {
        for (uint32 i = 1; i < MAX_ASSETS; ++i) {
            if (m_assets[i].ref_count > 0 && strncmp(m_assets[i].name, name, 64) == 0) {
                m_assets[i].ref_count++;
                return {i, m_assets[i].generation};
            }
        }
        VOID_ASSERT(m_free_count > 0);
        const uint32 id = m_free_indices[--m_free_count];
        strncpy(m_assets[id].name, name, 63);
        m_assets[id].name[63] = '\0';
        m_assets[id].ref_count = 1;
        m_assets[id].texture = void_texture_load(window, name);
        /*if (!m_assets[id].texture_ptr) {
            VOID_LOG_ERROR("Impossible de charger la texture : %s", name);
        }*/
        return {id, m_assets[id].generation};
    }

    void asset_manager::release_texture(const asset handle) {
        if (!is_valid(handle)) return;
        const uint32 id = handle.id;
        m_assets[id].ref_count--;
        if (m_assets[id].ref_count == 0) {
            if (m_assets[id].texture) {
                void_texture_destroy(m_assets[id].texture);
                m_assets[id].texture = nullptr;
            }
            m_assets[id].generation++;
            m_free_indices[m_free_count++] = id;
        }
    }
    bool asset_manager::is_valid(const asset handle) const {
        if (handle.id == 0 || handle.id >= MAX_ASSETS) return false;
        return m_assets[handle.id].generation == handle.generation && m_assets[handle.id].ref_count > 0;
    }
    const VoidTexture* asset_manager::get_texture(const asset handle) const {
        if (!is_valid(handle)) return nullptr;
        return m_assets[handle.id].texture;
    }
}
