#include "DebugSystem.hpp"

#include "entities.hpp"

namespace ray {

DebugSystem::DebugSystem(GameManager& gm) :
    Base(),
    _gm(&gm)
{
    //ctor
}

DebugSystem::~DebugSystem()
{
    //dtor
}

#ifdef DEBUG
void DebugSystem::update(const vector<Event>& events) {
    RenderWindow& window = *this->_gm->getRenderWindow();
    for (const Event& e : events) {
        if (e.type == Event::EventType::KeyPressed) {
            if (e.key.code == Keyboard::Key::E) {
                Vector2f worldpos =
                    window.mapPixelToCoords(Mouse::getPosition(window));
                // Where in the world will the bullet fly to?

                entities::createEntity("Enemy", worldpos.x, worldpos.y, 8.0f);
            }
        }
    }
}
#else
void DebugSystem::update(const vector<Event>& events) {
    // Do nothing in Release mode
}
#endif // DEBUG

}
