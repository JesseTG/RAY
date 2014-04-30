#include "AISystem.hpp"

namespace ray {

const ComponentFilter AISystem::FILTER = ComponentFilter()
    .requires<AIComponent>();

AISystem::AISystem(LuaContext& lua) : Base(FILTER), _lua(&lua)
{
    //ctor
}

AISystem::~AISystem()
{
    //dtor
}

void AISystem::update() {
    for (Entity& e : this->getEntities()) {
        AIComponent& aic = e.getComponent<AIComponent>();
        aic.update(e);
    }
}

}
