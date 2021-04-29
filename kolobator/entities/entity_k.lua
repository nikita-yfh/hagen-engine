function entity_k.init(a)
	a.weapon="ak47"
	entity_k.speed=10
	entity_k.motor_speed=40
end
function entity_k.update(a)
	local j=a:joint("joint")
	local b=a:body("body")
	if(a.health==0) then 
		return 1
	else
		if(math.abs(player.x-a.x)<3) then
			if(player.x+2<a.x and b.vx>-entity_k.speed and game.interval(20)) then
				j.motor=1
				j.motor_speed=-entity_k.motor_speed
				j.max_torque=30000
				b:apply_center_impulse(-24,0)
			elseif(player.x-2>a.x and b.vx<entity_k.speed and game.interval(20)) then
				j.motor=1
				j.motor_speed=entity_k.motor_speed
				j.max_torque=30000
				b:apply_center_impulse(24,0)
			else
				j.motor=0
			end
			a:focus_on_entity(player)
			a:fire2()
		end
	end
end