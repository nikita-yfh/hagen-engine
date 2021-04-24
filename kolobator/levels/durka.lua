function level:init()
end
function level:update()
	level_trigger(body("_end"),"durka")
	if(world.bl_all_collide(body("bullet_sensor"))) then durka_door=true end
	if(durka_door==true) then
		if(body("door").y < 17.5) then
			body("door").vy=0
		else
			body("door").vy=-1
		end
	end
	if(world.eb_collide(entity("player"),body("platform"))) then
		body("platform").vx=1
	end
	if(body("platform").x > 62.5) then
		body("platform").vx=0
	end
	if(world.eb_all_collide(entity("player"),body("patrony"))) then 
		player:bullet("27mm"):add(5)
		world.destroy_body(body("patrony"))
	end
end
