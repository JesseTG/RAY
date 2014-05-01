function Enemy_die(e)
    local sound = SFML.Audio.Sound.Get("boom1")
    sound:play()
    e:addRemovalComponent(RemovalComponent.new())
    return e
end

function create_Entity_Enemy(target, x, y, r)
    local e = Anax.Entity.new()
    local bodydef = Box2D.BodyDef.new()

    local fixturedef = Box2D.FixtureDef.new()
    local position = Box2D.Vector.new(x, y)
    local shape = Box2D.Shape.Circle.new(r)
    local circle = SFML.CircleShape.new(r, 16)

    circle.origin = SFML.Vector.new(r, r)
    circle.position =  SFML.Vector.new(position.x, position.y)
    circle.fillColor = SFML.Color.Green

    bodydef.active = true
    bodydef.allowSleep = false
    bodydef.type = Box2D.BodyType.Dynamic
    bodydef.position = position
    bodydef.fixedRotation = false

    fixturedef:setShape(shape)
    fixturedef.restitution = .3

    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)

    local pc = PositionComponent.new(x, y)
    local rc = RenderableComponent.new(circle)
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local tbrc = TractorBeamRepellableComponent.new()
    local hc = HealthComponent.new(20)
    local aic = AIComponent.new(AI.Wander)
    local efc = EntityFollowComponent.new(target, 3)
    hc.onDeath = Enemy_die


    e:addPositionComponent(pc)
    e:addRenderableComponent(rc)
    e:addPhysicsBodyComponent(pbc)
    e:addTractorBeamRepellableComponent(tbrc)
    e:addPhysicsFixtureComponent(pfc)
    e:addAIComponent(aic)
    e:addEntityFollowComponent(efc)
    e:addHealthComponent(hc)

    return e
end

function create_Entity_MouseCircle(r)
    local e = Anax.Entity.new()
    local circle = SFML.CircleShape.new(r)

    circle.outlineThickness = 1
    circle.outlineColor = SFML.Color.White
    circle.fillColor = SFML.Color.Transparent
    circle.origin = SFML.Vector.new(r, r)

    local rc = RenderableComponent.new(circle, 1000)
    local pc = PositionComponent.new(0, 0)
    local mfcc = MouseFollowControlComponent.new()
    e:addRenderableComponent(rc)
    e:addPositionComponent(pc)
    e:addMouseFollowControlComponent(mfcc)

    return e
end

function create_Entity_KeyboardCircle(target, r, x, y)
    local e = Anax.Entity.new()
    local bodydef = Box2D.BodyDef.new()
    local fixturedef = Box2D.FixtureDef.new()

    local shape = Resource.Shape.Get("ship")
    local ship = shape.group
    local shape = shape.physics_shapes[2]

    bodydef.allowSleep = false
    bodydef.type = Box2D.BodyType.Dynamic
    bodydef.active = true
    bodydef.fixedRotation = true
    bodydef.linearDamping = 1.5

    fixturedef:setShape(shape)
    fixturedef.density = .5
    bodydef.position = Box2D.Vector.new(x, y)

    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)

    local rc = RenderableComponent.new(ship, 0)
    local fwcc = FourWayControlComponent.new()
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local fec = FaceEntityComponent.new(target)
	local tc = TimerComponent.new(2)
	local hc = HealthComponent.new(PLAYER_MAX_HEALTH)
	hc.onDeath = Enemy_die
    fwcc.targetSpeed = 1

    e:addRenderableComponent(rc)
    e:addFourWayControlComponent(fwcc)
    e:addPhysicsBodyComponent(pbc)
    e:addPhysicsFixtureComponent(pfc)
    e:addFaceEntityComponent(fec)
	e:addHealthComponent(hc)
	e:addTimerComponent(tc)

    return e
end


function create_Entity_TractorBeam(
    face_target,
    follow_target,
    starting_width,
    starting_angle,
    length,
    force
)
    local e = Anax.Entity.new()
    local followpbc = follow_target:getPhysicsBodyComponent()
    local tbdef = Box2D.FixtureDef.new()
    tbdef.density = 0
    tbdef.isSensor = true


    local tbshape = Box2D.Shape.Rectangle.new(
        starting_width / 2,
        length / 2,
        Box2D.Vector.new(length / 2, 0),
        math.pi / 2
    )
    tbdef:setShape(tbshape)

    local rectangle = SFML.RectangleShape.new(length * 64, starting_width * 64)
    rectangle.fillColor = SFML.Color.Blue
    rectangle.origin = SFML.Vector.new(0, 64 * (starting_width / 2))

    local fixture = followpbc.body:CreateFixture(tbdef)

    local rc = RenderableComponent.new(rectangle, -10)
    local fec = FaceEntityComponent.new(face_target)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local tbc = TractorBeamComponent.new()
    tbc.starting_width = starting_width
    tbc.starting_angle = starting_angle
    tbc.length = length
    tbc.force = force

    e:addRenderableComponent(rc)
    e:addFaceEntityComponent(fec)
    e:addPhysicsFixtureComponent(pfc)
    e:addTractorBeamComponent(tbc)

    return e
end

function create_Entity_Asteroid(rank, x, y)
    local e = Anax.Entity.new()
    local bodydef = Box2D.BodyDef.new()
    local fixturedef = Box2D.FixtureDef.new()

    local shape = Resource.Shape.Get("asteroid")
    local asteroid = shape.group

    bodydef.allowSleep = false
    bodydef.type = Box2D.BodyType.Dynamic
    bodydef.active = true
    bodydef.fixedRotation = false
    bodydef.linearDamping = 1.5
    bodydef.angularDamping = .5

    fixturedef:setShape(shape.physics_shapes[2])
    fixturedef.density = 2
    bodydef.position = Box2D.Vector.new(x, y)

    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)

    local rc = RenderableComponent.new(asteroid, 0)
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
	local hc = HealthComponent.new(20)
	local tbrc = TractorBeamRepellableComponent.new()
	hc.onDeath = Enemy_die

    e:addRenderableComponent(rc)
    e:addPhysicsBodyComponent(pbc)
    e:addPhysicsFixtureComponent(pfc)
	e:addHealthComponent(hc)
	e:addTractorBeamRepellableComponent(tbrc)

    return e
end

function create_Entity_Text(text, x, y)
    local e = Anax.Entity.new()

    return e
end
