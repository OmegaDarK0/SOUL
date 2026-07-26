#ifndef SOUL_ECS_HPP
#define SOUL_ECS_HPP

#include "void.h"

#ifdef _WIN32
    #ifdef SOUL_BUILD_DLL
        #define SOUL_API __declspec(dllexport)
    #else
        #define SOUL_API __declspec(dllimport)
    #endif
#else
    #define SOUL_API
#endif

namespace soul {
    using entity = uint32;
    using component = uint8;
    using signature = uint32;
    constexpr component MAX_COMPONENTS = 32;
    constexpr entity NULL_ENTITY = 0xFFFFFFFF;
    namespace internal {
        inline component get_component_id() {
            static component id = 0;
            return id++;
        }
        SOUL_API component get_id_for_type(const char* type_name);
    }
    template <typename>
    component get_component_type() {
#if defined(_MSC_VER)
        static const component type = internal::get_id_for_type(__FUNCSIG__);
#else
        static const component type = internal::get_id_for_type(__PRETTY_FUNCTION__);
#endif
        return type;
    }
    class SOUL_API entity_manager {
        entity* m_available_entities = nullptr;
        uint32 m_available_count = 0;
        signature* m_signatures = nullptr;
        entity m_max_entities = 0;
    public:
        void init(entity max_entities);
        entity create_entity();
        void destroy_entity(entity e);
        void set_signature(entity e, signature sig) const;
        [[nodiscard]] signature get_signature(entity e) const;
        [[nodiscard]] const signature* get_signatures() const;
        [[nodiscard]] entity get_max_entities() const;
    };
    struct i_component_array {
        virtual ~i_component_array() = default;
        virtual void entity_destroyed(entity e) = 0;
    };
    template <typename T>
    class component_array : public i_component_array {
        T* m_components = nullptr;
        entity* m_entity_to_index = nullptr;
        entity* m_index_to_entity = nullptr;
        uint32 m_size = 0;
        entity m_max_entities = 0;
    public:
        void init(const entity max_entities) {
            m_max_entities = max_entities;
            m_components = static_cast<T*>(void_arena_alloc(max_entities * sizeof(T), alignof(T)));
            m_entity_to_index = static_cast<entity*>(void_arena_alloc(max_entities * sizeof(entity), alignof(entity)));
            m_index_to_entity = static_cast<entity*>(void_arena_alloc(max_entities * sizeof(entity), alignof(entity)));
            for (entity i = 0; i < max_entities; ++i) {
                m_entity_to_index[i] = NULL_ENTITY;
            }
        }
        void insert_data(const entity e, T component) {
            VOID_ASSERT(e < m_max_entities);
            VOID_ASSERT(m_entity_to_index[e] == NULL_ENTITY);
            const uint32 new_index = m_size;
            m_entity_to_index[e] = new_index;
            m_index_to_entity[new_index] = e;
            m_components[new_index] = component;
            m_size++;
        }
        void remove_data(const entity e) {
            VOID_ASSERT(e < m_max_entities);
            VOID_ASSERT(m_entity_to_index[e] != NULL_ENTITY);
            const uint32 removed_index = m_entity_to_index[e];
            const uint32 last_index = m_size - 1;
            m_components[removed_index] = m_components[last_index];
            const entity last_entity = m_index_to_entity[last_index];
            m_entity_to_index[last_entity] = removed_index;
            m_index_to_entity[removed_index] = last_entity;
            m_entity_to_index[e] = NULL_ENTITY;
            m_size--;
        }
        [[nodiscard]] T& get_data(const entity e) {
            VOID_ASSERT(e < m_max_entities);
            VOID_ASSERT(m_entity_to_index[e] != NULL_ENTITY);
            return m_components[m_entity_to_index[e]];
        }
        void entity_destroyed(const entity e) override {
            if (e < m_max_entities && m_entity_to_index[e] != NULL_ENTITY) {
                remove_data(e);
            }
        }
    };
    class SOUL_API registry {
        entity_manager m_entity_manager;
        i_component_array* m_component_arrays[MAX_COMPONENTS] = {nullptr};
        uint32 m_max_entities = 0;
        void* m_contexts[32] = {nullptr};
        template <typename>
        static uint32 get_context_type_id() {
            static uint32 s_id = s_context_counter++;
            return s_id;
        }
        static inline uint32 s_context_counter = 0;
        template <typename T>
        component_array<T>* get_component_array() {
            const component type = get_component_type<T>();
            VOID_ASSERT(type < MAX_COMPONENTS);
            VOID_ASSERT(m_component_arrays[type] != nullptr);
            return static_cast<component_array<T>*>(m_component_arrays[type]);
        }
    public:
        void init(uint32 max_entities);
        entity create_entity();
        void destroy_entity(entity e);
        template <typename T>
        void set_context(const T& data) {
            const uint32 id = get_context_type_id<T>();
            VOID_ASSERT(id < 32);
            if (m_contexts[id] == nullptr) {
                m_contexts[id] = void_arena_alloc(sizeof(T), alignof(T));
                VOID_ASSERT(m_contexts[id] != nullptr);
            }
            *static_cast<T*>(m_contexts[id]) = data;
        }
        template <typename T>
        [[nodiscard]] const T& get_context() const {
            const uint32 id = get_context_type_id<T>();
            VOID_ASSERT(id < 32);
            VOID_ASSERT(m_contexts[id] != nullptr);
            return *static_cast<T*>(m_contexts[id]);
        }
        template <typename T>
        void register_component() {
            const component type = get_component_type<T>();
            VOID_ASSERT(type < MAX_COMPONENTS);
            VOID_ASSERT(m_component_arrays[type] == nullptr);
            auto* ptr = static_cast<component_array<T>*>(void_arena_alloc(sizeof(component_array<T>), alignof(component_array<T>)));
            new(ptr) component_array<T>();
            ptr->init(m_max_entities);
            m_component_arrays[type] = ptr;
        }
        template <typename T>
        void add_component(entity e, T component) {
            get_component_array<T>()->insert_data(e, component);
            signature sig = m_entity_manager.get_signature(e);
            sig |= 1 << get_component_type<T>();
            m_entity_manager.set_signature(e, sig);
        }
        template <typename T>
        void remove_component(entity e) {
            get_component_array<T>()->remove_data(e);
            signature sig = m_entity_manager.get_signature(e);
            sig &= ~(1 << get_component_type<T>());
            m_entity_manager.set_signature(e, sig);
        }
        template <typename T>
        [[nodiscard]] T& get_component(entity e) {
            return get_component_array<T>()->get_data(e);
        }
        template <typename T>
        [[nodiscard]] bool has_component(const entity e) const {
            signature sig = m_entity_manager.get_signature(e);
            return (sig & 1 << get_component_type<T>()) != 0;
        }
        template <typename...>
        class view {
            const signature* m_signatures;
            uint32 m_max_entities;
            signature m_mask;
        public:
            view(const signature* sigs, const uint32 max_e, const signature mask)
                : m_signatures(sigs), m_max_entities(max_e), m_mask(mask) {}
            struct iterator {
                const signature* signatures;
                uint32 max_entities;
                signature mask;
                entity current;
                iterator(const signature* sigs, const uint32 max_e, const signature m, const entity start)
                    : signatures(sigs), max_entities(max_e), mask(m), current(start) {
                    while (current < max_entities && (signatures[current] & mask) != mask) current++;
                }
                entity operator*() const { return current; }
                iterator& operator++() {
                    current++;
                    while (current < max_entities && (signatures[current] & mask) != mask) current++;
                    return *this;
                }
                bool operator!=(const iterator& other) const { return current != other.current; }
            };
            iterator begin() const { return iterator(m_signatures, m_max_entities, m_mask, 0); }
            iterator end() const { return iterator(m_signatures, m_max_entities, m_mask, m_max_entities); }
        };
        template <typename... ComponentTypes>
        [[nodiscard]] view<ComponentTypes...> get_view() const {
            signature mask = 0;
            ((mask |= 1 << get_component_type<ComponentTypes>()), ...);
            VOID_ASSERT(mask != 0);
            return view<ComponentTypes...>(m_entity_manager.get_signatures(), m_entity_manager.get_max_entities(), mask);
        }
    };
}

#endif //SOUL_ECS_HPP
