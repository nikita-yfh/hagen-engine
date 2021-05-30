function level_trigger(body,name)
	print(entity("player"))
	if(entity("player")~=nil and world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end
function play(name,x,y)
	if(entity("player")~=nil) then
		sound.play_distance(name,math.sqrt(math.pow(player.x-x,2)+math.pow(player.y-y,2)))
	else
		sound.play(name)
	end
end
function init_main_menu()
    loadlevel("ground1")
end
function new_game()
    loadlevel("podval")
end