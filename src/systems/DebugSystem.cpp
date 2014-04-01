#include "DebugSystem.hpp"

#include "entities.hpp"

namespace ray {

DebugSystem::DebugSystem(RenderWindow& window, GameManager& gm) :
    Base(),
    _window(&window),
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
    for (const Event& e : events) {
        if (e.type == Event::EventType::KeyPressed) {
            if (e.key.code == Keyboard::Key::E) {
                Vector2f worldpos =
                    this->_window->mapPixelToCoords(Mouse::getPosition(*this->_window));
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
