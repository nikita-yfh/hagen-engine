function health.init(a,val)
	a.userdata.heal=val
end
function health.update(a)
	if(world.eb_all_collide(entity("player"),a) and player.health~=player.max_health and game.press_key("action")) then
		player:heal(a.userdata.heal)
		sound.play("bullet_box.flac")
		return true
	end
	return false
end