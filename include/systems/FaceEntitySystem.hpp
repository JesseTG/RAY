#ifndef FACEENTITYSYSTEM_HPP
#define FACEENTITYSYSTEM_HPP

#include <anax/anax.hpp>

namespace ray {

using anax::ComponentFilter;

class FaceEntitySystem : public anax::System<FaceEntitySystem>
{
    public:
        FaceEntitySystem();
        virtual ~FaceEntitySystem();
        void update();
    private:
        static const ComponentFilter FILTER;
};
}

#endif // FACEENTITYSYSTEM_HPP
