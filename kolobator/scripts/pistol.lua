function pistol.init(w)
end
function pistol.update(w,e)
	if(game.key("fire") and game.interval(0)) then
		body=e:body("body")
		bullet=world.create_body("bullet","",e.x,e.y)
		bullet.angle=game.camera.angle
		bullet:apply_center_impulse(math.cos(game.camera.angle)*200,math.sin(game.camera.angle)*200);
		body:apply_center_impulse(-math.cos(game.camera.angle)*200,-math.sin(game.camera.angle)*200);
	end
end
