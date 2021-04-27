function pistol.init(w)
	w.dx=0.1
	w.dy=0.1
	w.bullet1="9mm"
	w.bullet2="27mm"
end
function pistol.fire1(w,e)
	local body=e:body("body")
	local bullet=world.create_body("bullet_9mm","",e.x,e.y)
	bullet.angle=e.weapon_angle
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	bullet:apply_center_impulse(math.cos(game.camera.angle)*100,math.sin(game.camera.angle)*100);
	body:apply_center_impulse(-math.cos(game.camera.angle)*100,-math.sin(game.camera.angle)*100);
	sound.play("pistol.flac")
	return 1
end

function pistol.fire3(w,e)
	local body=e:body("body")
	local bullet=world.create_body("bullet_27mm","",e.x,e.y)
	bullet.angle=e.weapon_angle
	bullet.userdata.create_time=game.timer
	bullet.userdata.entity=e.id
	bullet:apply_center_impulse(math.cos(game.camera.angle)*1000,math.sin(game.camera.angle)*1000);
	body:apply_center_impulse(-math.cos(game.camera.angle)*1000,-math.sin(game.camera.angle)*1000);
	sound.play("pistol.flac")
	return 1
end
