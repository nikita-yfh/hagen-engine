function box_27mm_5.update(a)
	if(world.eb_all_collide(entity("player"),a) and not bullet("27mm"):full() and game.press_key("action")) then
		bullet("27mm"):add(5)
		sound.play("bullet_box.flac")
		return true
	end
	return false
end