function loadLevel(id)
    local shapes = Resource.Shape.Get(id)
    local music = SFML.Audio.Music.Load("ray01")
    music.volume = 50
    music:play()
    for s = 1, #shapes.graphics_shapes do
        local e = Anax.Entity.new()
        local rc = RenderableComponent.new(shapes.graphics_shapes[s])
        local pc = PositionComponent.new(shapes:startPosition(s).x, shapes:startPosition(s).y)

        e:addRenderableComponent(rc)
        e:addPositionComponent(pc)
        if shapes.physics_shapes[s] then
            local bdef = Box2D.BodyDef.new()
            bdef.awake = true
            bdef.active = true
            bdef.position = Box2D.Vector.new(pc.x / 64, pc.y / 64)
            bdef.type = Box2D.BodyType.Static
            bdef.fixedRotation = true

            local fdef = Box2D.FixtureDef.new()
            fdef:setShape(shapes.physics_shapes[s])
            local body = Box2D.Body.new(bdef)
            local fixture = body:CreateFixture(fdef)
            local pbc = PhysicsBodyComponent.new(body, e)
            local pfc = PhysicsFixtureComponent.new(fixture, e)
            e:addPhysicsBodyComponent(pbc)
            e:addPhysicsFixtureComponent(pfc)
        end
    end
end

function load_Level_collide()

end
