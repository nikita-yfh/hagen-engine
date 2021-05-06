function rg6.init(w)
	w.dx=0.34
	w.dy=0.1
	w.bullet1="grenade"
end
function rg6.fire2(w,e)
	if(not game.interval(50)) then return 0 end
	local bullet=world.create_bullet(e,"grenade",300)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("shot.flac")
	return 1
end