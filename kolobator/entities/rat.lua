function rat.init(a)
	graphics.preload("rat/1-1.png")
	graphics.preload("rat/2-1.png")
	graphics.preload("rat/1-2.png")
	graphics.preload("rat/2-2.png")
	a.health=10
	a.userdata.effect="blood"
end
function rat.update(a)
	local j=a:joint("joint")
	local b=a:body("body")
	local f=b:fixture("body_fixture")
	if(a.health==0) then
		play("rat/die.flac",a.x,a.y)
		graphics.effect("big_blood",a.x,a.y)
		return true
	end
	local angle=b.angle%(math.pi*2)
	if(angle>math.pi/4 and angle<math.pi*2-math.pi/4) then
		if(angle>math.pi) then
			b:apply_torque(80)
		else
			b:apply_torque(-80)
		end
	elseif(entity("player")) then
		local dx=player.x-a.x
		local dy=player.y-a.y
		local distance=math.sqrt(dx*dx+dy*dy)
		local body
		world.raycast(player.x,player.y,a.x,a.y,
			function(fixture,px,py,nx,ny,fraction)
				local e2=world.who(fixture.body)
				if((e2 and (e2.id==a.id or e2.id==player.id)) or fixture.mode~=0) then return -1 end
				body=fixture.body
				return fraction
			end)
		if(not body and distance<3 and a.vx<4) then
			if(distance>0.4) then
				local v=1;
				if(player.x<a.x) then
					j.motor=1
					j.motor_speed=-40
					j.max_torque=30000
				else
					j.motor=1
					j.motor_speed=40
					j.max_torque=30000
					v=2;
				end
				if game.interval(100) then
					if(f.texture=="rat/1-1.png" or f.texture=="rat/1-2.png") then
						f.texture="rat/2-"..v..".png"
					else
						f.texture="rat/1-"..v..".png"
					end
				end
			elseif(world.lb_collide(a:body("wheel"))) then
				play("rat/atack"..math.random(1,1)..".flac",a.x,a.y)
				b:apply_center_impulse(0,-40)
			elseif(world.eb_all_collide(player,b) and game.interval(300)) then
				player:harm(5)
			end
		else
			j.motor=1
			j.motor_speed=0
			j.max_torque=30000
		end
	end
	if(game.interval(100) and math.random(1,30)==1) then
		play("rat/squeak"..math.random(1,4)..".flac",a.x,a.y)
	end
end
