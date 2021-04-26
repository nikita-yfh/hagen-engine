function bullet_9mm.init(a)
	a.userdata.create_time=game.timer
end
function bullet_9mm.update(a)
	local entity=world.sb_all_collide(a)
	if(entity and a.v > 50 and a.userdata.create_time+5 < game.timer)then
		entity:harm(5)
		return true
	elseif(a.userdata.create_time+5000 < game.timer) then
		return true
	end
	return false
end
