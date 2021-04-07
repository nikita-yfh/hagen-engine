function kolobator.init(a)
	a.weapon="pistol"
	player:bullet("9mm").count=27
	player:bullet("27mm").count=1000
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
	if(game.key("fire1") and player:bullet(weapon(a.weapon).bullet1).count > 0) then
		player:bullet(weapon(a.weapon).bullet1).count = player:bullet(weapon(a.weapon).bullet1).count-a:fire1()
	elseif(game.key("fire2") and player:bullet(weapon(a.weapon).bullet2).count > 0) then
		player:bullet(weapon(a.weapon).bullet2).count = player:bullet(weapon(a.weapon).bullet2).count-a:fire2()
	end
	a.weapon_angle=game.camera.angle
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
end
