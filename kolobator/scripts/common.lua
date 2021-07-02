if(entity('player')) then
	player=entity('player')
end
function level_trigger(body,name)
	if(player and world.eb_all_collide(player,body)) then
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
function init_main_menu()
    loadlevel("ground1")
end
function crash(body,value)
	if(body.userdata.health) then
		body.userdata.health=body.userdata.health-value
	end
end