function bullet_9mm.update(a)
	local entity=world.sb_all_collide(a)
	local body=world.lb_collide(a)
	if(entity and a.userdata.entity ~= entity.id)then
		crash_e(entity,a.x,a.y,10)
		return true
	elseif(body) then
		crash(body,a.x,a.y,10)
		return true
	elseif(a.userdata.create_time+5000 < game.time) then
		return true
	end
	return false
end
