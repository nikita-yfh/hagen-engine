subtitles_time=300
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
    loadlevel('ground1')
end
function crash(body,value)
	if(body.userdata.health) then
		body.userdata.health=body.userdata.health-value
	end
end
function subtitles(str)
	local space=true
	local counter=0
	if(str:sub(#str,#str) ~= ' ') then str=str..' ' end
	for c in str:gmatch('.') do
		if(c==' ' or c=='.' or c==',' or c==';' or c==':' or c=='!' or c=='?') then
			if(space==false) then
				counter=counter+1
			end
			space=true
		else
			space=false
		end
	end
	local time=counter*subtitles_time
	print('Subtitles time: '..time..' '..str)
	text.add_subtitles(str,time)
	return time
end
function subtitles_trigger(body,name)
	if(player and world.eb_all_collide(player,body)) then
		subtitles(name)
		world.destroy_body(body);
	end
end
