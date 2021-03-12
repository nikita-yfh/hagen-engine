function Level:init()
print("Loading 'default' level...")
end
--e
function Level:update()
print("Player:	x=" .. entity("player").x .. ";	y=" .. entity("player").y)
end
