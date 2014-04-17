#ifndef ENTITYSTATE_HPP
#define ENTITYSTATE_HPP

#include <anax/anax.hpp>

#include <cassert>
#include <stdexcept>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>

#include "fsm.hpp"

namespace util {
using std::is_base_of;
using std::logic_error;
using std::unordered_map;
using anax::BaseComponent;
using anax::ComponentTypeList;
using anax::Entity;
using anax::detail::TypeId;

/**
 * Represents a set of @c Components that an @c Entity might have in a given
 * game state.  For example, if the player character can become invincible
 * (e.g. after spawning, after grabbing an item, etc.), he might be in an
 * Invincibility state where he doesn't have a hypothetical @c DamageComponent.
 *
 * Ideally might be used in some sort of Stateful component.
 *
 * EntityStates cannot be created on their own; they must be instantiated with
 * an owning EntityStateMachine.
 */
class EntityState
{
    friend class EntityStateMachine;

    public:
        ~EntityState();

        /**
         * Adds a @c Component to this EntityState.
         *
         * Do not pass in a @c Component that's being allocated with @c new!
         * Any @c Components in here will already be owned by the @c World that
         * owns whatever @c Entity controls this EntityState anyway.
         */
        template<class T>
        EntityState& add(T* component) {
            static_assert(is_base_of<BaseComponent, T>::value, "Must extend anax::Component");

            assert(component != nullptr);
            this->_components[T::GetTypeId()] = component;

            return *this;
        }

        template<class T>
        EntityState& add() {
            static_assert(is_base_of<BaseComponent, T>::value, "Must extend anax::Component");

            this->_components.set(T::GetTypeId());

            return *this;
        }

        /**
         * Returns a reference to a @c Component.
         */
        template<class T>
        T& get() const {
            static_assert(is_base_of<BaseComponent, T>::value, "Must extend anax::Component");
            if (!this->has<T>()) {
                throw logic_error("Component type not found");
            }

            return this->_entity.getComponent<T>();
        }

        /**
         * Returns @c true if this EntityState has a given @c Component type.
         *
         * @tparam T The @c Component type whose existence we're querying. Will
         * throw a compile-time error if it does not derive from
         * @c anax::BaseComponent.
         */
        template<class T>
        bool has() const {
            static_assert(is_base_of<BaseComponent, T>::value, "Must extend anax::Component");

            return this->_components[T::GetTypeId()];
        }
    private:
        vector<shared_ptr<EntityState>> nextStates;
        Entity _entity;
        ComponentTypeList _components;
        unordered_map<TypeId, std::shared_ptr<BaseComponent>> _cache;

        EntityState(const Entity&);
};
}

#endif // ENTITYSTATE_HPP
