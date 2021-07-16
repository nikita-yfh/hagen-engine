function Body.update(b)
	if(b.userdata.health and b.userdata.health<=0) then
		if(b.userdata.killfunc) then
			b.userdata.killfunc(b)
		end
		return 1
	end
end
