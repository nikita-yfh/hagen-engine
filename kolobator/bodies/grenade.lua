function grenade.update(a)
	local entity=world.sb_all_collide(a)
	if((entity~=nil and a.v > 10 and a.userdata.entity ~= entity.id)
or world.lb_collide(a) or a.userdata.create_time+5000 < game.timer)then
		world.explosion_callbacks(a.x,a.y,5,10000,
		function(e,d)
			e:harm(5/d)
			if(e.health<0) then e.health=0 end
		end)
		graphics.effect("explosion",a.x,a.y)
		play("boom.flac",a.x,a.y)
		return true
	end
	return false
end