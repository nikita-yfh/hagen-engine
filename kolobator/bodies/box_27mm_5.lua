function box_27mm_5.update(a)
	if(world.eb_all_collide(entity("player"),a) and game.press_key("action")) then
		player:bullet("27mm"):add(5)
		return true
	end
	return false
end