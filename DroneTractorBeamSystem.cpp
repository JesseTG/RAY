#include "DroneTractorBeamSystem.hpp"
#include "components.hpp"
#include "random.h"
#include <iostream>

namespace ray{

const ComponentFilter DroneTractorBeamSystem::FILTER = ComponentFilter()
        .requires<PositionComponent, TractorBeamComponent>();

DroneTractorBeamSystem::DroneTractorBeamSystem(TractorBeamRepellingListener& listener) :
        Base(FILTER), _listener(&listener)
{
    //ctor
}

DroneTractorBeamSystem::~DroneTractorBeamSystem()
{
    //dtor
}

void DroneTractorBeamSystem::update(){
    for(Entity& e : this->getEntities()){
        TractorBeamComponent& tbc = e.getComponent<TractorBeamComponent>();
        RenderableComponent& ren = e.getComponent<RenderableComponent>();
        PhysicsFixtureComponent& fix = e.getComponent<PhysicsFixtureComponent>();
        Shape* shape = static_cast<Shape*>(ren.drawable.get());

        b2Filter filter;


    }
}
}
