function kolobator.init(a)
	a.weapon="pistol"
end
function kolobator.update(a)
	local max_speed=10
	local motor_speed=40
	local j=a:joint("joint")
	local b=a:body("body")
	if(game.key("left") and b.vx>-max_speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=-motor_speed
		j.max_torque=30000
		b:apply_center_impulse(-24,0)
	elseif(game.key("right") and b.vx<max_speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=motor_speed
		j.max_torque=30000
		b:apply_center_impulse(24,0)
	else
		j.motor=0
	end
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
	world.collide(a,body("sensor"))
end
