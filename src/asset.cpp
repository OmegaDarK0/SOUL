#include "asset.hpp"

namespace soul {
    void asset_manager::init(const uint32 max_assets) {
        m_max_assets = max_assets;
        m_assets = static_cast<texture_asset *>(void_arena_alloc(max_assets * sizeof(texture_asset), alignof(texture_asset)));
        m_free_indices = static_cast<uint32 *>(void_arena_alloc(max_assets * sizeof(uint32), alignof(uint32)));
        VOID_ASSERT(m_assets != nullptr && m_free_indices != nullptr);
        for (uint32 i = 1; i < m_max_assets; ++i) {
            m_free_indices[m_max_assets - 1 - i] = i;
            m_assets[i].generation = 1;
            m_assets[i].ref_count = 0;
            m_assets[i].texture = nullptr;
            m_assets[i].name[0] = '\0';
        }
        m_free_count = m_max_assets - 1;
    }
    void asset_manager::exit() const {
        for (uint32 i = 1; i < m_max_assets; ++i) {
            if (m_assets[i].ref_count > 0 && m_assets[i].texture) {
                void_texture_destroy(m_assets[i].texture);
                m_assets[i].ref_count = 0;
                m_assets[i].texture = nullptr;
            }
        }
    }
    asset asset_manager::load_texture(const VoidWindow* window, const char* name) {
        for (uint32 i = 1; i < m_max_assets; ++i) {
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
        if (handle.id == 0 || handle.id >= m_max_assets) return false;
        return m_assets[handle.id].generation == handle.generation && m_assets[handle.id].ref_count > 0;
    }
    const VoidTexture* asset_manager::get_texture(const asset handle) const {
        if (!is_valid(handle)) return nullptr;
        return m_assets[handle.id].texture;
    }
}
