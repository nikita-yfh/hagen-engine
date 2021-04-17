function kolobator.init(a)
	a.weapon="pistol"
	player:bullet("9mm").count=27
	player:bullet("9mm").max=27
	player:bullet("27mm").count=10
	player:bullet("27mm").max=10
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
		if(game.press_key("jump") and world.lb_collide(a:body("wheel"))) then
			b:apply_center_impulse(0,-2000)
		end
	end
	if(game.key("1")) then 
		entity("player").weapon="knife"
	elseif(game.key("2")) then
		entity("player").weapon="pistol"
	elseif(game.key("3")) then
		entity("player").weapon="ak47"
	end
	if(player:bullet(weapon(a.weapon).bullet1).count > 0) then
		if(game.press_key("fire1")) then
			player:bullet(weapon(a.weapon).bullet1).count = player:bullet(weapon(a.weapon).bullet1).count-a:fire1()
		elseif(game.key("fire1")) then
			player:bullet(weapon(a.weapon).bullet1).count = player:bullet(weapon(a.weapon).bullet1).count-a:fire2()
		end
	end
	if(player:bullet(weapon(a.weapon).bullet2).count > 0) then
		if(game.press_key("fire2")) then
			player:bullet(weapon(a.weapon).bullet2).count = player:bullet(weapon(a.weapon).bullet2).count-a:fire3()
		elseif(game.key("fire2")) then
			player:bullet(weapon(a.weapon).bullet2).count = player:bullet(weapon(a.weapon).bullet2).count-a:fire4()
		end
	end
	a.weapon_angle=game.camera.angle
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
end
