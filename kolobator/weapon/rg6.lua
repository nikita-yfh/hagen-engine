function rg6.init(w,e)
	w.dx=0.34
	w.dy=0.1
	w.bullet1="grenade"
	w.texture="weapon/rg6.png"
end
function rg6.fire1(w,e)
	local bullet=world.create_bullet(e,"grenade",150)
	bullet.userdata.create_time=game.time
	bullet.userdata.entity=e.id
	play("shot.flac",e.x,e.y)
	return 1
end