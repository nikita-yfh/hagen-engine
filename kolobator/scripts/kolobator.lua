function kolobator.init(a)
end
function kolobator.update(a)
	j=a:joint("joint")
	if(game.key("left")) then
		j.motor=1
		j.motor_speed=-20
		j.max_torque=100000000
	elseif(game.key("right")) then
		j.motor=1
		j.motor_speed=20
		j.max_torque=100000000
	else
		j.motor=0
	end
	if(game.camera.locked) then
		game.camera.center_body(a:body("body"))
	end
	
end
