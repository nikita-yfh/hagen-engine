create_time={}
first_entity={}
function level_trigger(body,name)
	if(world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end
function Level.update()
	print(game.timer)
end