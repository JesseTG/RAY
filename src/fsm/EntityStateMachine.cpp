#include "EntityStateMachine.hpp"

namespace util {
EntityStateMachine::EntityStateMachine(Entity& entity) : _entity(entity)
{
    //ctor
}

EntityStateMachine::~EntityStateMachine()
{
    //dtor
}

Entity EntityStateMachine::entity() const {
    return this->_entity;
}

EntityState& EntityStateMachine::_create_state(const int state) {
    this->_states[state] = unique_ptr<EntityState>(new EntityState(this->_entity));
    return *(this->_states[state]);
}

EntityState& EntityStateMachine::_change_state(const int state) {
    if (!this->_has(state)) {
        throw logic_error("Entity state not found");
    } else if (this->_states[state] == *(this->_current_state)) {
        return **(this->_current_state);
    }
    else {
        /*
        auto& old_bitset = (*(this->_current_state))->_components;
        auto& new_bitset = this->_states[state]->_components;
        int size = old_bitset.size();
        for (int i = 0; i < size; ++i) {
        if (old_bitset[i] && !new_bitset[i]) {
            // If the new state does not have the current component...
        }
        }
        */
    }
}

bool EntityStateMachine::_has(const int state) const {
    return this->_states.find(state) != this->_states.end();
}

}
