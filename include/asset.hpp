#ifndef SOUL_ASSET_HPP
#define SOUL_ASSET_HPP

#include <string_view>
#include "void.h"

namespace soul {
    struct asset {
        uint32 id = 0;
        uint32 generation = 0;
        [[nodiscard]] bool is_valid() const { return id != 0; }
        bool operator==(const asset& other) const {
            return id == other.id && generation == other.generation;
        }
    };
    struct texture_asset {
        char name[64];
        VoidTexture* texture;
        uint32 ref_count;
        uint32 generation;
    };
    struct asset_manager {
        static constexpr uint32 MAX_ASSETS = 1024;
        void init();
        void exit();
        asset load_texture(const VoidWindow* window, const char* name);
        void release_texture(asset handle);
        [[nodiscard]] bool is_valid(asset handle) const;
        [[nodiscard]] const VoidTexture* get_texture(asset handle) const;
    private:
        texture_asset m_assets[MAX_ASSETS] = {};
        uint32 m_free_indices[MAX_ASSETS] = {};
        uint32 m_free_count = 0;
    };
}

#endif //SOUL_ASSET_HPP
