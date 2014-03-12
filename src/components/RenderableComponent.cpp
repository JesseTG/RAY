#include <utility>

#include "RenderableComponent.hpp"

namespace ray {
void RenderableComponent::luaInit(LuaContext& lua) {
    typedef variant<CircleShape*, RectangleShape*, ConvexShape*, Sprite*, Text*, VertexArray*> drawtypes;
    lua.writeFunction(
        "RenderableComponent",
        "new",
    [](drawtypes types) {

        Drawable* drawptr;
        switch (types.which()) {
            case 0:
                drawptr = get<CircleShape*>(types);
                break;
            case 1:
                drawptr = get<RectangleShape*>(types);
                break;
            case 2:
                drawptr = get<ConvexShape*>(types);
                break;
            case 3:
                drawptr = get<Sprite*>(types);
                break;
            case 4:
                drawptr = get<Text*>(types);
                break;
            case 5:
                drawptr = get<VertexArray*>(types);
                break;
        }

        return new RenderableComponent(drawptr);
    });

    lua.registerMember("transformable", &RenderableComponent::transformable);
    lua.registerMember("drawable", &RenderableComponent::drawable);
    lua.registerMember("layer", &RenderableComponent::layer);
}
}
