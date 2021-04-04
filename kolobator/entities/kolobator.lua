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
		if(game.key("jump") and world.collide(a)) then
			b:apply_center_impulse(0,-200)
		end
	end
	if(game.key("fire1")) then
		a:fire1()
	elseif(game.key("fire2")) then
		a:fire2()
	end
	a.weapon_angle=game.camera.angle
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
end
