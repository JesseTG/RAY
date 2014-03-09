print(Anax.Entity)
print(Anax.Entity.new)


p = PositionComponent.new()
r = RenderableComponent.new()
print(p)
print(r)

p.x = 40
p.y = 40
print("p.position", p.position)
print("p.x", p.x)
print("p.y", p.y)

entity:addPositionComponent(p)
entity:addRenderableComponent(r)

p.x = 80
p.y = 80
print("pos.x", p.x)
print("pos.y", p.y)
