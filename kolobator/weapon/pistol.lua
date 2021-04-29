function pistol.init(w)
	w.dx=0.1
	w.dy=0.1
	w.bullet1="9mm"
end
function pistol.fire1(w,e)
	local bullet=world.create_bullet(e,"bullet_9mm",100)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("shot.flac")
	return 1
end
