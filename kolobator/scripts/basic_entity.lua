function basic_entity.init(a)
end
function basic_entity.update(a)
	local max_speed=5
	local motor_speed=20
	local j=a:joint("joint")
	local b=a:body("body")
	local player=entity("player")
	if(player.x<a.x-3 and b.vx>-max_speed) then
		j.motor=1
		j.motor_speed=-motor_speed
		j.max_torque=10000
		b:apply_center_impulse(-8,0)
	elseif(player.x>a.x+3 and b.vx<max_speed) then
		j.motor=1
		j.motor_speed=motor_speed
		j.max_torque=10000
		b:apply_center_impulse(8,0)
	else
		j.motor=0
	end

end
