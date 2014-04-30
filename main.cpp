#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>
#include <utility>
#include <memory>

#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include <SFGUI/SFGUI.hpp>

#include "fsm.hpp"
#include "entities.hpp"
#include "listeners.hpp"
#include "managers.hpp"
#include "ai.hpp"


int main()
{
    using std::vector;
    using std::string;
    using std::make_pair;
    using std::chrono::milliseconds;
    using sf::Event;
    using sf::ContextSettings;
    using sf::RenderWindow;
    using sf::VideoMode;
    using sf::View;
    using namespace util;
    using namespace ray;

    GameManager gm;
    RenderWindow& window = *gm.getRenderWindow();
    // Create the main window
    entities::setLuaState(gm.getLuaContext());

    entities::initBaseTypes(gm);
    entities::initManagerTypeBindings(gm);
    entities::initComponentLuaBindings(gm);
    ai::initAIBindings(gm);

    vector<Event> events;
    bool focused = true;
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            events.push_back(event);

            // Close window : exit
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::LostFocus:
                    focused = false;
                    break;
                case Event::GainedFocus:
                    focused = true;
                    break;
                case Event::KeyPressed:
                    break;
                default:
                    ; // nop
            }
        }

        gm.getStateMachine()->update(events);

        events.clear();
    }
}
