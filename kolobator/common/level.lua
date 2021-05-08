function level_trigger(body,name)
	print(entity("player"))
	if(entity("player")~=nil and world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end