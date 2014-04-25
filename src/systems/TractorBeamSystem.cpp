#include "TractorBeamSystem.hpp"

#include "components.hpp"
#include <iostream>
#include <cmath>

namespace ray {
    using std::exp;
const ComponentFilter TractorBeamSystem::FILTER = ComponentFilter()
        .requires<PositionComponent, TractorBeamComponent>();

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
            //diff.Normalize();
            //diff *= exp(diff.Length());
            std::cout << diff.x << " " << diff.y << std::endl;

            targetbody->ApplyForceToCenter(tbc.force * diff, true);
        }
    }

}
}

