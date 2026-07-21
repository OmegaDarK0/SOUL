#include "ecs.hpp"

namespace soul {
    entity_manager::entity_manager() {
        for (entity i = 0; i < MAX_ENTITIES; ++i) {
            m_entities.push(i);
        }
    }
    entity entity_manager::create_entity() {
        VOID_ASSERT(m_entity_count < MAX_ENTITIES);
        const entity e = m_entities.front();
        m_entities.pop();
        m_entity_count++;
        return e;
    }
    void entity_manager::destroy_entity(const entity e) {
        VOID_ASSERT(e < MAX_ENTITIES);
        m_signatures[e] = 0;
        m_entities.push(e);
        m_entity_count--;
    }
    void entity_manager::set_signature(const entity e, const signature sig) {
        VOID_ASSERT(e < MAX_ENTITIES);
        m_signatures[e] = sig;
    }
    signature entity_manager::get_signature(const entity e) const {
        VOID_ASSERT(e < MAX_ENTITIES);
        return m_signatures[e];
    }
    const std::array<signature, MAX_ENTITIES>& entity_manager::get_signatures() const {
        return m_signatures;
    }
    entity registry::create_entity() {
        return m_entity_manager.create_entity();
    }
    void registry::destroy_entity(const entity e) {
        m_entity_manager.destroy_entity(e);
        for (const auto &[fst, snd] : m_component_arrays) {
            snd->entity_destroyed(e);
        }
    }
}
