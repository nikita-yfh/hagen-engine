﻿function bullet_27mm.update(a)
	local entity=world.sb_all_collide(a)
	if(entity and a.v > 50 and a.userdata.entity ~= entity.id)then
		entity:harm(20)
		return true
	elseif(a.userdata.create_time+5000 < game.timer) then
		return true
	end
	return false
end
