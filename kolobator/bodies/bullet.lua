function bullet.init(a)
	create_time[a.id]=game.timer
end
function bullet.update(a)
	local entity=world.sb_all_collide(a)
	if(entity and a.v > 50 and create_time[a.id]+5 < game.timer)then
		entity:harm(5)
		return true
	elseif(create_time[a.id]+5000 < game.timer) then
		return true
	end
	return false
end
