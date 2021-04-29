function health_20.update(a)
	if(world.eb_all_collide(entity("player"),a) and player.health~=player.max_health and game.press_key("action")) then
		player:heal(20)
		return true
	end
	return false
end