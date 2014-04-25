#ifndef AISYSTEM_H
#define AISYSTEM_H

#include <anax/anax.hpp>
#include <LuaContext.hpp>
#include "components.hpp"

namespace ray {
using anax::ComponentFilter;
using anax::System;


class AISystem : public anax::System<AISystem>
{
    public:
        AISystem(LuaContext& lua);
        virtual ~AISystem();
        void update();
    protected:
    private:
        LuaContext* _lua;
        static const ComponentFilter FILTER;
};
}

#endif // AISYSTEM_H
