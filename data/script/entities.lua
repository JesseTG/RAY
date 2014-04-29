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

    fixturedef:setShape(shape)
    fixturedef.restitution = 2

    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)

    local pc = PositionComponent.new(x, y)
    local rc = RenderableComponent.new(circle)
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local tbrc = TractorBeamRepellableComponent.new()
    local aic = AIComponent.new(AI.Wander)
    local efc = EntityFollowComponent.new(target, 10)


    e:addPositionComponent(pc)
    e:addRenderableComponent(rc)
    e:addPhysicsBodyComponent(pbc)
    e:addTractorBeamRepellableComponent(tbrc)
    e:addPhysicsFixtureComponent(pfc)
    e:addAIComponent(aic)
    e:addEntityFollowComponent(efc)

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
    local b2circle = Box2D.Shape.Circle.new()

    local shape = Resource.Shape.Get("ship")
    local ship = shape.group

    bodydef.allowSleep = false
    bodydef.type = Box2D.BodyType.Dynamic
    bodydef.active = true
    bodydef.fixedRotation = true

    fixturedef:setShape(b2circle)
    fixturedef.density = 2
    b2circle.radius = r
    bodydef.position = Box2D.Vector.new(x, y)

    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)

    local rc = RenderableComponent.new(ship, 0)
    local pc = PositionComponent.new(x, y)
    local vc = VelocityComponent.new()
    local fwcc = FourWayControlComponent.new()
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local fec = FaceEntityComponent.new(target)
	local hc = HealthComponent.new(PLAYER_MAX_HEALTH, PLAYER_INIT_DEFENCE)

    e:addRenderableComponent(rc)
    e:addPositionComponent(pc)
    e:addVelocityComponent(vc)
    e:addFourWayControlComponent(fwcc)
    e:addPhysicsBodyComponent(pbc)
    e:addPhysicsFixtureComponent(pfc)
    e:addFaceEntityComponent(fec)
	e:addHealthComponent(hc)

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
    local followpc = follow_target:getPositionComponent()
    local followpbc = follow_target:getPhysicsBodyComponent()
    local targetpos = follow_target:getPositionComponent()
    local targetpbc = follow_target:getPhysicsBodyComponent()
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

    local rectangle = SFML.RectangleShape.new(length, starting_width);
    rectangle.fillColor = SFML.Color.Blue
    rectangle.origin = SFML.Vector.new(0, starting_width / 2)

    local fixture = followpbc.body:CreateFixture(tbdef)

    e:addRenderableComponent(RenderableComponent.new(rectangle, -10))
    e:addPositionComponent(PositionComponent.new(targetpos.x, targetpos.y))
    e:addEntityFollowComponent(EntityFollowComponent.new(follow_target))
    e:addFaceEntityComponent(FaceEntityComponent.new(face_target))
    e:addPhysicsFixtureComponent(PhysicsFixtureComponent.new(fixture, e));
    e:addTractorBeamComponent(TractorBeamComponent.new())

    return e
end

function create_Entity_Text(text, x, y)
    local e = Anax.Entity.new()

    return e
end
