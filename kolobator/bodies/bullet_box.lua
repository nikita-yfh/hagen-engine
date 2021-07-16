function bullet_box.init(a,name,count)
	a.userdata.bullet_name=name
	a.userdata.bullet_count=count
end
function bullet_box.update(a)
	if(world.eb_all_collide(entity("player"),a) and not bullet(a.userdata.bullet_name):full() and game.press_key("action")) then
		bullet(a.userdata.bullet_name):add(a.userdata.bullet_count)
		sound.play("bullet_box.flac")
		return true
	end
	return false
end
