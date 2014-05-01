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
	
	if id == "collide" then
		load_Level_collide()
	elseif id == "star" then
		load_Level_star()
	end
end

spawningPoints = {}
spawningFreqs = {}
spawningAmt = 0

function getSpawningPoint(index)
	return spawningPoints[index]
end

function getSpawningFreq(index)
	return spawningFreqs[index]
end

function load_Level_collide()
	spawningAmt = 2
	spawningPoints = {}
	spawningFreqs = {}
	spawningPoints[1] = create_Entity_Spawning_Point(2, 2)
	spawningFreqs[1] = 500
	spawningPoints[2] = create_Entity_Spawning_Point(4, 4)
	spawningFreqs[2] = 400
end

function load_Level_star()
	spawningAmt = 0
	spawningPoints = {}
	spawningFreqs = {}
end
