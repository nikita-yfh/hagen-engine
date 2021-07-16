function Body.update(b)
	if(b.userdata.health and b.userdata.health<=0) then
		if(b.userdata.killfunc) then
			b.userdata.killfunc(b)
		end
		if(b.userdata.killsound) then
			play(b.userdata.killsound,b.x,b.y)
		end
		return 1
	end
end
