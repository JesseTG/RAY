#ifndef DESTRUCTIONLISTENER_HPP
#define DESTRUCTIONLISTENER_HPP

#include <Box2D/Box2D.h>

namespace ray {
class DestructionListener : public b2DestructionListener
{
    public:
        DestructionListener();
        virtual ~DestructionListener();
        void SayGoodbye(b2Joint* joint) override;
        void SayGoodbye(b2Fixture* fixture) override;
    protected:
    private:
};
}

#endif // DESTRUCTIONLISTENER_HPP
