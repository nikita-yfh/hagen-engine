function level_trigger(body,name)
	print(entity("player"))
	if(entity("player")~=nil and world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end
function play(name,x,y)
	if(player) then
		sound.play_distance(name,math.sqrt(math.pow(player.x-x,2)+math.pow(player.y-y,2)))
	else
		sound.play(name)
	end
end