#include "ecs.hpp"

namespace soul {
    namespace internal {
        component get_id_for_type(const char* type_name) {
            static const char* registered_names[MAX_COMPONENTS] = {nullptr};
            static component next_id = 0;
            for (component i = 0; i < next_id; ++i) {
                if (strcmp(registered_names[i], type_name) == 0) {
                    return i;
                }
            }
            VOID_ASSERT(next_id < MAX_COMPONENTS);
            registered_names[next_id] = type_name;
            return next_id++;
        }
    }
    void entity_manager::init(const entity max_entities) {
        m_max_entities = max_entities;
        m_signatures = static_cast<signature*>(void_arena_alloc(max_entities * sizeof(signature), alignof(signature)));
        m_available_entities = static_cast<entity*>(void_arena_alloc(max_entities * sizeof(entity), alignof(entity)));
        m_available_count = max_entities;
        for (entity i = 0; i < max_entities; ++i) {
            m_signatures[i] = 0;
            m_available_entities[i] = max_entities - 1 - i;
        }
    }
    entity entity_manager::create_entity() {
        VOID_ASSERT(m_available_count > 0);
        m_available_count--;
        return m_available_entities[m_available_count];
    }
    void entity_manager::destroy_entity(const entity e) {
        VOID_ASSERT(e < m_max_entities);
        m_signatures[e] = 0;
        m_available_entities[m_available_count] = e;
        m_available_count++;
    }
    void entity_manager::set_signature(const entity e, const signature sig) const {
        VOID_ASSERT(e < m_max_entities);
        m_signatures[e] = sig;
    }
    signature entity_manager::get_signature(const entity e) const {
        VOID_ASSERT(e < m_max_entities);
        return m_signatures[e];
    }
    const signature* entity_manager::get_signatures() const {
        return m_signatures;
    }
    entity entity_manager::get_max_entities() const {
        return m_max_entities;
    }
    void registry::init(const entity max_entities) {
        m_max_entities = max_entities;
        m_entity_manager.init(max_entities);
        for (auto & m_component_array : m_component_arrays) {
            m_component_array = nullptr;
        }
    }
    entity registry::create_entity() {
        return m_entity_manager.create_entity();
    }
    void registry::destroy_entity(const entity e) {
        m_entity_manager.destroy_entity(e);
        for (const auto & m_component_array : m_component_arrays) {
            if (m_component_array != nullptr) {
                m_component_array->entity_destroyed(e);
            }
        }
    }
}
