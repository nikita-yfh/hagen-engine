function pistol.init(w)
	w.dx=0.1
	w.dy=0.1
	w.bullet1="9mm"
	w.bullet2="27mm"
end
function pistol.fire1(w,e)
	if(not game.interval(100)) then return 0 end
	local body=e:body("body")
	bullet=world.create_body("bullet","",e.x,e.y)
	bullet.angle=e.weapon_angle
	bullet:apply_center_impulse(math.cos(game.camera.angle)*100,math.sin(game.camera.angle)*100);
	body:apply_center_impulse(-math.cos(game.camera.angle)*100,-math.sin(game.camera.angle)*100);
	return 1
end

function pistol.fire2(w,e)
	if(not game.interval(100)) then return 0 end
	local body=e:body("body")
	bullet=world.create_body("big_bullet","",e.x,e.y)
	bullet.angle=e.weapon_angle
	bullet:apply_center_impulse(math.cos(game.camera.angle)*1000,math.sin(game.camera.angle)*1000);
	body:apply_center_impulse(-math.cos(game.camera.angle)*1000,-math.sin(game.camera.angle)*1000);
	return 1
end