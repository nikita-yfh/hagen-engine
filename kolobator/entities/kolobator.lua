function kolobator.init(a)
	graphics.preload("kolobator1.png")
	graphics.preload("kolobator2.png")
	kolobator.speed=15
	kolobator.motor_speed=30
	kolobator.jump_impulse=1350
	kolobator.jump=0
end
function kolobator.update(a)
	local j=a:joint("joint")
	local b=a:body("body")
	if(a.health==0) then
		graphics.effect("big_blood",a.x,a.y)
		return true
	end
	a.weapon.angle=game.camera.angle
	game.camera.center_input(a.x,a.y)
	if(game.press_key("jump")) then kolobator.jump=true end
	if(game.release_key("jump")) then kolobator.jump=false end
	if(game.key("left") and b.vx>-kolobator.speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=-kolobator.motor_speed
		j.max_torque=30000
		b:apply_center_impulse(-6,0)
	elseif(game.key("right") and b.vx<kolobator.speed and game.interval(20)) then
		j.motor=1
		j.motor_speed=kolobator.motor_speed
		j.max_torque=30000
		b:apply_center_impulse(6,0)
	elseif(game.key("down")) then
		j.motor=1
		j.motor_speed=0
		j.max_torque=30000
	else
		j.motor=0
		j.max_torque=0
 		if(kolobator.jump==true and world.lb_collide(a:body("wheel"))) then
			b:apply_center_impulse(0,-kolobator.jump_impulse)
			kolobator.jump=false
		end
	end
	if(a.userdata.weapons) then
		if(a.userdata.weapons["knife"] and game.key("1")) then 
			a:set_weapon("knife")
		elseif(a.userdata.weapons["pistol"] and game.key("2")) then
			a:set_weapon("pistol")
		elseif(a.userdata.weapons["ak47"] and game.key("3")) then
			a:set_weapon("ak47")
		elseif(a.userdata.weapons["rg6"] and game.key("4")) then
			a:set_weapon("rg6")
		end
	end
	local b1=a.weapon.bullet1;
	if(a.weapon.name ~= "" and b1=="" or bullet(b1).count ~= 0) then
		if(game.press_key("fire1")) then
			bullet(b1):del(a:fire1())
		elseif(game.key("fire1")) then
			bullet(b1):del(a:fire2())
		end
	end
	if(a.weapon.angle>math.pi*0.5 and a.weapon.angle<math.pi*1.5) then
		b:set_texture("body_fixture","kolobator2.png")
	else
		b:set_texture("body_fixture","kolobator1.png")
	end
	return false
end
