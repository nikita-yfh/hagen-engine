function kolobator.init(a)
	graphics.preload("kolobator1.png")
	graphics.preload("kolobator2.png")
	a.weapon="knife"
	bullet("9mm").count=100
	bullet("9mm").max=100
	bullet("grenade").count=6
	bullet("grenade").max=6
	kolobator.speed=10
	kolobator.motor_speed=40
	kolobator.jump_impulse=2000
end
function kolobator.update(a)
	local j=a:joint("joint")
	local b=a:body("body")
	if(game.key("left") and b.vx>-kolobator.speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=-kolobator.motor_speed
		j.max_torque=30000
		b:apply_center_impulse(-24,0)
	elseif(game.key("right") and b.vx<kolobator.speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=kolobator.motor_speed
		j.max_torque=30000
		b:apply_center_impulse(24,0)
	elseif(game.key("down")) then
		j.motor=1
		j.motor_speed=0
		j.max_torque=30000
	else
		j.motor=0
		if(game.press_key("jump") and world.lb_collide(a:body("wheel"))) then
			b:apply_center_impulse(0,-kolobator.jump_impulse)
		end
	end
	if(game.key("1")) then 
		entity("player").weapon="knife"
	elseif(game.key("2")) then
		entity("player").weapon="pistol"
	elseif(game.key("3")) then
		entity("player").weapon="ak47"
	elseif(game.key("4")) then
		entity("player").weapon="rg6"
	end
	if(bullet(weapon(a.weapon).bullet1).count > 0) then
		if(game.press_key("fire1")) then
			bullet(weapon(a.weapon).bullet1).count = bullet(weapon(a.weapon).bullet1).count-a:fire1()
		elseif(game.key("fire1")) then
			bullet(weapon(a.weapon).bullet1).count = bullet(weapon(a.weapon).bullet1).count-a:fire2()
		end
	end
	if(bullet(weapon(a.weapon).bullet2).count > 0) then
		if(game.press_key("fire2")) then
			bullet(weapon(a.weapon).bullet2).count = bullet(weapon(a.weapon).bullet2).count-a:fire3()
		elseif(game.key("fire2")) then
			bullet(weapon(a.weapon).bullet2).count = bullet(weapon(a.weapon).bullet2).count-a:fire4()
		end
	end
	a.weapon_angle=game.camera.angle
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
	if(a.weapon_angle>math.pi*0.5 and a.weapon_angle<math.pi*1.5) then
		b:set_texture("body_fixture","kolobator2.png")
	else
		b:set_texture("body_fixture","kolobator1.png")
	end
end
