function pistol.init(w)
	w.texture="pistol.png"
end
function pistol.update(w,e)
	if(game.key("fire") and game.interval(100)) then
		body=e:body("body")
		bullet=world.create_body("bullet","",e.x,e.y)
		bullet.angle=game.camera.angle
		bullet:apply_center_impulse(math.cos(game.camera.angle)*50,math.sin(game.camera.angle)*50);
		body:apply_center_impulse(-math.cos(game.camera.angle)*50,-math.sin(game.camera.angle)*50);
		game.camera.x=game.camera.x+50
		game.camera.y=game.camera.y+50
	end
end
