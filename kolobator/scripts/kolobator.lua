timer=0
function kolobator.init(a)
end
function kolobator.update(a)
	local max_speed=5
	local motor_speed=20
	local j=a:joint("joint")
	local b=a:body("body")
	if(game.key("left") and b.vx>-max_speed) then
		j.motor=1
		j.motor_speed=-motor_speed
		j.max_torque=10000
		b:apply_center_impulse(-16,0)
	elseif(game.key("right") and b.vx<max_speed) then
		j.motor=1
		j.motor_speed=motor_speed
		j.max_torque=10000
		b:apply_center_impulse(16,0)
	else
		j.motor=0
	end
	if(game.key("3") and game.timer-timer>100) then
		timer=game.timer
		bullet=world.create_entity("kolobator","",a.x,a.y-0.5)
		--bullet:apply_center_impulse(math.cos(game.camera.angle)*50,math.sin(game.camera.angle)*50);
		--b:apply_center_impulse(-math.cos(game.camera.angle)*50,-math.sin(game.camera.angle)*50);
	end
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
	world.set_gravity(math.cos(game.camera.angle)*10,math.sin(game.camera.angle)*10)
end
