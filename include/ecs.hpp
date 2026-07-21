#ifndef SOUL_ECS_HPP
#define SOUL_ECS_HPP

#include <array>
#include <queue>
#include <memory>
#include <unordered_map>
#include "void.h"

namespace soul {
    using entity = uint32;
    constexpr entity MAX_ENTITIES = 128;
    using component = uint8;
    constexpr component MAX_COMPONENTS = 32;
    using signature = uint32;
    namespace internal {
        inline component get_component_id() {
            static component id = 0;
            return id++;
        }
    }
    template <typename>
    component get_component_type() {
        static const component type = internal::get_component_id();
        return type;
    }
    class entity_manager {
        std::queue<entity> m_entities;
        std::array<signature, MAX_ENTITIES> m_signatures{};
        uint32 m_entity_count{};
    public:
        entity_manager();
        entity create_entity();
        void destroy_entity(entity e);
        void set_signature(entity e, signature sig);
        [[nodiscard]] signature get_signature(entity e) const;
        [[nodiscard]] const std::array<signature, MAX_ENTITIES>& get_signatures() const;
    };
    struct i_component_array {
        virtual ~i_component_array() = default;
        virtual void entity_destroyed(entity e) = 0;
    };
    template <typename T>
    class component_array : public i_component_array {
        std::array<T, MAX_ENTITIES> m_components;
        std::unordered_map<entity, size_t> m_entity_to_index;
        std::unordered_map<size_t, entity> m_index_to_entity;
        size_t m_size{};
    public:
        void insert_data(const entity e, T component) {
            VOID_ASSERT(!m_entity_to_index.contains(e));
            m_components[m_size] = component;
            m_entity_to_index[e] = m_size;
            m_index_to_entity[m_size] = e;
            m_size++;
        }
        void remove_data(const entity e) {
            VOID_ASSERT(m_entity_to_index.contains(e));
            m_size--;
            size_t idx = m_entity_to_index[e];
            T component = m_components[m_size];
            m_components[idx] = component;
            const entity tmp = m_index_to_entity[m_size];
            m_entity_to_index[tmp] = idx;
            m_index_to_entity[idx] = tmp;
            m_entity_to_index.erase(e);
            m_index_to_entity.erase(m_size);
        }
        [[nodiscard]] T &get_data(const entity e) {
            VOID_ASSERT(m_entity_to_index.contains(e));
            return m_components[m_entity_to_index[e]];
        }
        void entity_destroyed(const entity e) override {
            if (m_entity_to_index.contains(e)) {
                remove_data(e);
            }
        }
    };
    class registry {
        entity_manager m_entity_manager;
        std::unordered_map<component, std::shared_ptr<i_component_array>> m_component_arrays;
        template <typename T>
        std::shared_ptr<component_array<T>> get_component_array() {
            const component type = get_component_type<T>();
            VOID_ASSERT(m_component_arrays.contains(type));
            return std::static_pointer_cast<component_array<T>>(m_component_arrays[type]);
        }
    public:
        entity create_entity();
        void destroy_entity(entity e);
        template <typename T>
        void register_component() {
            const component type = get_component_type<T>();
            VOID_ASSERT(!m_component_arrays.contains(type));
            m_component_arrays[type] = std::make_shared<component_array<T>>();
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
        [[nodiscard]] T &get_component(entity e) {
            return get_component_array<T>()->get_data(e);
        }
        template <typename...>
        class view {
            const std::array<signature, MAX_ENTITIES>& m_signatures;
            signature m_mask;
        public:
            view(const std::array<signature, MAX_ENTITIES>& sigs, const signature mask)
                : m_signatures(sigs), m_mask(mask) {}
            struct iterator {
                const std::array<signature, MAX_ENTITIES>& signatures;
                signature mask;
                entity current;
                iterator(const std::array<signature, MAX_ENTITIES>& sigs, const signature m, const entity start)
                    : signatures(sigs), mask(m), current(start) {
                    while (current < MAX_ENTITIES && (signatures[current] & mask) != mask) {
                        current++;
                    }
                }
                entity operator*() const { return current; }
                iterator& operator++() {
                    current++;
                    while (current < MAX_ENTITIES && (signatures[current] & mask) != mask) {
                        current++;
                    }
                    return *this;
                }
                bool operator!=(const iterator& other) const {
                    return current != other.current;
                }
            };
            iterator begin() const { return iterator(m_signatures, m_mask, 0); }
            iterator end() const { return iterator(m_signatures, m_mask, MAX_ENTITIES); }
        };
        template <typename... ComponentTypes>
        [[nodiscard]] view<ComponentTypes...> get_view() const {
            signature mask = 0;
            ((mask |= 1 << get_component_type<ComponentTypes>()), ...);
            VOID_ASSERT(mask != 0);
            return view<ComponentTypes...>(m_entity_manager.get_signatures(), mask);
        }
    };
}

#endif //SOUL_ECS_HPP
