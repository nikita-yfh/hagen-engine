function level.newgame()
	if(entity("player")) then
		player.userdata.weapons={}
		player.userdata.weapons["knife"]=true
	end
end