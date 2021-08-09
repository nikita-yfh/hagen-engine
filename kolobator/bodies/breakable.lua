function breakable.init(b,health,effect,killsound,drop)
	b.userdata.health=health
	b.userdata.effect=effect
	b.userdata.killsound=killsound
	b.userdata.drop=drop
end
function breakable.update(b)
	if(b.userdata.health) then
		if(b.userdata.health<=0) then
			if(b.userdata.killfunc) then
				b.userdata.killfunc(b)
			end
			if(b.userdata.drop) then
				world.create_body(b.userdata.drop,b.x,b,y,nil)
			end
			if(b.userdata.killsound) then
				play(b.userdata.killsound,b.x,b.y)
			end
			return 1
		end
	end
end
