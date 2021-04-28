function health_20.update(a)
	if(world.eb_all_collide(entity("player"),a) and player.health~=100 and game.press_key("action")) then
		player.health=player.health+20
		return true
	end
	return false
end