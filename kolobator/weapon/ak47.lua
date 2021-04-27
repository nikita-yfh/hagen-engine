function ak47.init(w)
	w.dx=0.5
	w.dy=0.15
	w.bullet1="9mm"
	w.bullet2="27mm"
end
function ak47.fire2(w,e)
	if(not game.interval(100)) then return 0 end
	local bullet=world.create_bullet(w,e,"bullet_9mm",100)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("pistol.flac")
	return 1
end

function ak47.fire3(w,e)
	local bullet=world.create_bullet(w,e,"bullet_27mm",1000)
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	sound.play("pistol.flac")
	return 1
end
