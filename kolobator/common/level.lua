function level_trigger(body,name)
	if(world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end
