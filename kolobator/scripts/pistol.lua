function pistol.init(w)
	w.dx=0.1
	w.dy=0.1
end
function pistol.update(w,e)
	if(game.key("fire") and game.interval(100)) then
		print(e:body("body"))
	--	body=e:body("body")
		bullet=world.create_body("bullet","",e.x,e.y)
		--bullet.angle=game.camera.angle
	--	bullet:apply_center_impulse(math.cos(game.camera.angle)*100,math.sin(game.camera.angle)*100);
		--body:apply_center_impulse(-math.cos(game.camera.angle)*100,-math.sin(game.camera.angle)*100);
	end
end
