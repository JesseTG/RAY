#include <utility>

#include <boost/optional.hpp>
#include "RenderableComponent.hpp"

namespace ray {
using boost::optional;

RenderableComponent::~RenderableComponent() {
    assert(drawable.use_count() > 1);
    assert(drawable);
}

void RenderableComponent::luaInit(LuaContext& lua) {
    typedef variant<CircleShape*, RectangleShape*, ConvexShape*, Sprite*, Text*, VertexArray*, shared_ptr<Drawable>>
            drawtypes;
    lua.writeFunction(
        "RenderableComponent",
        "new",
    [](const drawtypes& types, optional<int> l) {

        shared_ptr<Drawable> drawptr;
        switch (types.which()) {
            case 0:
                drawptr.reset(get<CircleShape*>(types));
                break;
            case 1:
                drawptr.reset(get<RectangleShape*>(types));
                break;
            case 2:
                drawptr.reset(get<ConvexShape*>(types));
                break;
            case 3:
                drawptr.reset(get<Sprite*>(types));
                break;
            case 4:
                drawptr.reset(get<Text*>(types));
                break;
            case 5:
                drawptr.reset(get<VertexArray*>(types));
                break;
            default:
                drawptr = get<shared_ptr<Drawable>>(types);
                break;
        }

        if (l) {
            // If the scripter specified a layer...
            return new RenderableComponent(drawptr, *l);
        }
        else {
            return new RenderableComponent(drawptr);
        }
    });

    lua.registerMember("transformable", &RenderableComponent::transformable);
    lua.registerMember("drawable", &RenderableComponent::drawable);
    lua.registerMember("layer", &RenderableComponent::layer);
}
}
