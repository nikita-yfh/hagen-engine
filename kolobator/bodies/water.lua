function water.update(b)
	if(entity("player") and world.eb_all_collide(player,b) and game.interval(100)) then
		player:harm(1)
	end
end
