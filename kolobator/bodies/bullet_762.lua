﻿function bullet_762.update(a)
	local entity=world.sb_all_collide(a)
	if(entity and a.v > 50 and a.userdata.entity ~= entity.id)then
		entity:harm(5)
		graphics.effect("blood",a.x,a.y)
		return true
	elseif(a.userdata.create_time+5000 < game.time) then
		return true
	end
	return false
end
