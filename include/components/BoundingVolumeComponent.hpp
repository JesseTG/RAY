#ifndef BOUNDINGVOLUMECOMPONENT_HPP
#define BOUNDINGVOLUMECOMPONENT_HPP

#include <anax/anax.hpp>
#include <Box2D/Box2D.h>

namespace ray {

class BoundingVolumeComponent : public anax::Component<BoundingVolumeComponent>
{
    public:
        BoundingVolumeComponent() {}
        ~BoundingVolumeComponent() {}
    protected:
    private:
};

}
#endif // BOUNDINGVOLUMECOMPONENT_HPP
