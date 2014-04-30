#include "TractorBeamSystem.hpp"

#include "components.hpp"
#include <iostream>
#include <cmath>

namespace ray {
    using std::fabs;
    using std::exp;
    using std::atan2;
    using std::sin;
    using std::cos;
    using std::pow;
const ComponentFilter TractorBeamSystem::FILTER = ComponentFilter()
        .requires<PhysicsFixtureComponent, TractorBeamComponent>();

TractorBeamSystem::TractorBeamSystem(TractorBeamRepellingListener& listener) :
    Base(FILTER),
    _listener(&listener)
{
    //ctor
}

TractorBeamSystem::~TractorBeamSystem()
{
    //dtor
}

void TractorBeamSystem::update() {
    for (Entity& e : this->getEntities()) {
        TractorBeamComponent& tbc = e.getComponent<TractorBeamComponent>();
        RenderableComponent& ren = e.getComponent<RenderableComponent>();
        PhysicsFixtureComponent& fix = e.getComponent<PhysicsFixtureComponent>();
        Shape* shape = static_cast<Shape*>(ren.drawable.get());

        b2Filter filter;

        //TODO: Decouple color-changing from beam logic
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
            float distance = diff.Length();
            float force = fabs(tbc.length - distance);

            targetbody->ApplyForceToCenter(force * tbc.force * diff, true);
        }
    }

}
}

