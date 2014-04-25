#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

namespace sf {
using sf::Drawable;
using sf::Transformable;
using sf::RenderTarget;
using sf::Transform;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
class Group : public sf::Drawable, public sf::Transformable, public vector<shared_ptr<Drawable>>
{
    public:
        Group();

        template<class CollectionT>
        Group(const CollectionT& group) : vector<shared_ptr<Drawable>>(group.begin(), group.end()) {
        }

        virtual ~Group();

    protected:
        void draw(RenderTarget& target, RenderStates states) const override;
};
}

#endif // GROUP_HPP
