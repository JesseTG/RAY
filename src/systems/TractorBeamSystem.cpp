#include "TractorBeamSystem.hpp"

#include "components.hpp"

namespace ray {
const ComponentFilter TractorBeamSystem::FILTER = ComponentFilter()
    .requires<PositionComponent, TractorBeamComponent>();
TractorBeamSystem::TractorBeamSystem(const Entity& entity, RenderWindow& window) :
    _entity(entity),
    _window(&window),
    Base(FILTER)
{
    //ctor
}

TractorBeamSystem::~TractorBeamSystem()
{
    //dtor
}

void TractorBeamSystem::update() {
}
}
