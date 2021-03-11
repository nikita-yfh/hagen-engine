function Level:init()
print("Loading 'default' level...")
end
function Level:update()
print("Player:	x=" .. entity("player").x .. ";	y=" .. entity("player").y)
end
