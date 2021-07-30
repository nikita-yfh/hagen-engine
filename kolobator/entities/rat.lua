function rat.init(a)
	graphics.preload("rat/1.png")
	graphics.preload("rat/2.png")
end
function rat.update(a)
	local j=a:joint("joint")
	local b=a:body("body")
	local f=b:fixture("body_fixture")
	if(a.health==0) then
		graphics.effect("big_blood",a.x,a.y)
		return true
	end

	--[[if(a.angle) then

	else]]if(entity("player")) then
		local dx=player.x-a.x
		local dy=player.y-a.y
		local distance=math.sqrt(dx*dx+dy*dy)
		if(distance<3 and distance>0.3 and a.vx<4) then
			if(player.x<a.x) then
				j.motor=1
				j.motor_speed=-40
				j.max_torque=30000
			else
				j.motor=1
				j.motor_speed=40
				j.max_torque=30000
			end
			if game.interval(100) then
				if(f.texture=="rat/1.png") then
					f.texture="rat/2.png"
				else
					f.texture="rat/1.png"
				end
			end
		else
			f.texture="rat/1.png"
			j.motor=1
			j.motor_speed=0
			j.max_torque=30000
		end
	end
end
