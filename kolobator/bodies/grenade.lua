function grenade.update(a)
	local entity=world.sb_all_collide(a)
	if((entity and a.v > 10 and a.userdata.entity ~= entity.id)
			or world.lb_collide(a))then
		world.explosion_callbacks(a.x,a.y,5,10000,
		function(e,d)
			e:harm(5/d)
			if(e.health<0) then e.health=0 end
		end)
		graphics.effect("explosion",a.x,a.y)
		sound.play("boom.flac")
		return true
	elseif(a.userdata.create_time+5000 < game.timer) then
		graphics.effect("explosion",a.x,a.y)
		sound.play("boom.flac")
		return true
	end
	return false
end