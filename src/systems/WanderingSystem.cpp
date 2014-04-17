#include "WanderingSystem.hpp"
#include "components.hpp"
#include "util.hpp"

namespace ray{

using anax::ComponentFilter;

const ComponentFilter WanderingSystem::FILTER = ComponentFilter().requires<PositionComponent, WanderingComponent,
                                                                            PhysicsBodyComponent>();

WanderingSystem::WanderingSystem() : Base(FILTER)
{
    time.restart(sf::seconds(2));
}

bool setActive(bool _active){
    active = _active;
}

bool getActive(){
    return active;
}

void WanderingSystem::update(){
    //Get drone x
    //Get drone y
    //find random x within 128 px of x
    //find random y within 128 px of y
    //face the drone to that location
    //change the force vector of the drone
    //wait 2 seconds
    //repeat

    for (Entity& e : this->getEntities()) {
        if(e.getComponent<WanderingComponent>().active = true){
            if(time.isExpired()){
                PositionComponent& p = e.getComponent<PositionComponent>();

                x = p.position.x;
                y = p.position.y;

                randX = thor::random(x-64, x+64);
                randY = thor::random(y-64, y+64);

                time.restart(sf::seconds(2));

                float xDiff = x - randX;
                float yDiff = y - randY;
                theta = std::atan2(-yDiff, -xDiff);
            }

            PhysicsBodyComponent& pb = e.getComponent<PhysicsBodyComponent>();
            b2Vec2 pos((x-randX)/100, (y-randY)/100);
            pb.body->SetTransform(pb.body->GetPosition()+pos, theta);
        }
    }
}
}
