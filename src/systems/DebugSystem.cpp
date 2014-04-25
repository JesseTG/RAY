#include "DebugSystem.hpp"

#include "entities.hpp"
#include "states.hpp"

namespace ray {

DebugSystem::DebugSystem(GameManager& game) :
    Base(),
    _game(&game)
{
    //ctor
}

DebugSystem::~DebugSystem()
{
    //dtor
}

void DebugSystem::_change_level(const int l) {
    LevelInfo level;
    level.name = this->_game->getLevelManager()->getLevelName(l);
    this->_game->getStateMachine()->transition("advance", level);
}

#ifdef DEBUG
void DebugSystem::update(const vector<Event>& events) {
    RenderWindow& window = *this->_game->getRenderWindow();
    for (const Event& e : events) {
        if (e.type == Event::EventType::KeyPressed) {
            switch (e.key.code) {
                case Keyboard::Key::E: {
                        Vector2f worldpos =
                            window.mapPixelToCoords(Mouse::getPosition(window));
                        // Where in the world will the bullet fly to?

                        entities::createEntity("Enemy", this->_game->getPlayer(), worldpos.x, worldpos.y, 8.0f);
                    }
                    break;
                case Keyboard::Key::Num0:
                    if (e.key.control) {
                        this->_change_level(0);
                    }
                    break;
                case Keyboard::Key::Num1:
                    if (e.key.control) {
                        this->_change_level(1);
                    }
                    break;
                default:
                    break;
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

