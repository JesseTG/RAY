#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include <anax/anax.hpp>


namespace ray {
class AnimationSystem : public anax::System<AnimationSystem>
{
    public:
        AnimationSystem();
        virtual ~AnimationSystem();
        void update(const float dt);
    protected:
    private:
};
}

#endif // ANIMATIONSYSTEM_HPP
