#ifndef REMOVALCOMPONENT_H
#define REMOVALCOMPONENT_H

#include <anax/anax.hpp>

namespace ray {
using anax::Component;

struct RemovalComponent : public anax::Component<RemovalComponent>
{
};
}

#endif // REMOVALCOMPONENT_H
