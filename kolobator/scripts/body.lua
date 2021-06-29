function Body.update(b)
	if(b.userdata.health and b.userdata.health<=0) then 
		return 1
	end
end