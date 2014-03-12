function create_Entity_Enemy(x, y, r)
    local e = Anax.Entity.new()
    local bodydef = Box2D.BodyDef.new()
    local fixturedef = Box2D.FixtureDef.new()
    local position = Box2D.Vector.new()
    local shape = Box2D.Shape.Circle.new()
    local circle = SFML.CircleShape.new(r)
    
    position.x = x
    position.y = y
    shape.radius = r
    
    local origin = SFML.Vector.new()
    origin.x = r
    origin.y = r
    
    local pos = SFML.Vector.new()
    pos.x = position.x
    pos.y = position.y
    
    circle.origin = origin
    circle.position = pos
    circle.fillColor = SFML.Color.Green
    
    bodydef.active = true
    bodydef.allowSleep = false
    bodydef.type = Box2D.BodyType.Dynamic
    bodydef.position = position
    
    fixturedef:setShape(shape)
    
    local body = Box2D.Body.new(bodydef)
    local fixture = body:CreateFixture(fixturedef)
    
    local pc = PositionComponent.new(x, y)
    local rc = RenderableComponent.new(circle)
    local pbc = PhysicsBodyComponent.new(body, e)
    local pfc = PhysicsFixtureComponent.new(fixture, e)
    local tbrc = TractorBeamRepellableComponent.new()
    
    e:addPositionComponent(pc)
    e:addRenderableComponent(rc)
    e:addPhysicsBodyComponent(pbc)
    e:addTractorBeamRepellableComponent(tbrc)
    e:addPhysicsFixtureComponent(pfc)
    
    return e
end

function create_Entity_MouseCircle(r)
    local e = Anax.Entity.new()
    local circle = SFML.CircleShape.new(r)
    local origin = SFML.Vector.new()
    
    origin.x = r
    origin.y = r
    
    circle.outlineThickness = 1
    circle.outlineColor = SFML.Color.White
    circle.fillColor = SFML.Color.Transparent
    circle.origin = origin

    local rc = RenderableComponent.new(circle)
    local pc = PositionComponent.new(origin)
    local mfcc = MouseFollowControlComponent.new()
    
    e:addRenderableComponent(rc)
    e:addPositionComponent(pc)
    e:addMouseFollowControlComponent(mfcc)

    return e
end
