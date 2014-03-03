#include "TractorBeamSystem.hpp"

#include "components.hpp"
#include <iostream>

namespace ray {
const ComponentFilter TractorBeamSystem::FILTER = ComponentFilter()
        .requires<PositionComponent, TractorBeamComponent>();

TractorBeamSystem::TractorBeamSystem(TractorBeamRepellingListener& listener, Entity& playerbeam) :
    _listener(&listener),
    _player_beam(&playerbeam),
    Base(FILTER)
{
    //ctor
}

TractorBeamSystem::~TractorBeamSystem()
{
    //dtor
}

void TractorBeamSystem::update() {
    TractorBeamComponent& tbc = _player_beam->getComponent<TractorBeamComponent>();
    RenderableComponent& ren = _player_beam->getComponent<RenderableComponent>();
    PhysicsFixtureComponent& fix = _player_beam->getComponent<PhysicsFixtureComponent>();
    Shape* shape = static_cast<Shape*>(ren.drawable.get());

    b2Filter filter;

    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
        shape->setFillColor(Color::Blue);
        tbc.force = 1.0;
    }
    else if (Mouse::isButtonPressed(Mouse::Button::Right)) {
        shape->setFillColor(Color::Yellow);
        tbc.force = -1.0;
    }
    else {
        shape->setFillColor(Color::Transparent);
        filter.categoryBits = 0;
        tbc.force = 0;
    }
    fix.fixture->SetFilterData(filter);

    for (auto& i : this->_listener->getTractorBeamGrips()) {
        // i.first is the tractor beam fixture, i.second is the target fixture
        b2Body* beambody = i.first->GetBody();
        b2Body* targetbody = i.second->GetBody();

        b2Vec2 diff = targetbody->GetPosition() - beambody->GetPosition();

        targetbody->ApplyForceToCenter(tbc.force * diff, true);
    }
}
}
