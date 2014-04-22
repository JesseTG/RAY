#include "Group.hpp"

namespace sf {
Group::Group()
{
    //ctor
}

Group::~Group()
{
    //dtor
}

void Group::draw(RenderTarget &target, RenderStates states) const {
    states.transform = this->getTransform();

    for (auto ptr : *this) {
        target.draw(*ptr, states);
    }
}
}
