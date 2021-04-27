function pistol.init(w)
	w.dx=0.1
	w.dy=0.1
	w.bullet1="9mm"
	w.bullet2="27mm"
end
function pistol.fire1(w,e)
	local body=e:body("body")
	local bullet=world.create_bullet(w,e,"bullet_9mm",100)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("pistol.flac")
	return 1
end

function pistol.fire3(w,e)
	local bullet=world.create_bullet(w,e,"bullet_27mm",1000)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("pistol.flac")
	return 1
end
