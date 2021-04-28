function durka_man.init(a)
end
function durka_man.update(a)
	local b=a:body("body")
	if(a.health==0) then 
		graphics.effect("big_blood",a.x,a.y)
		return 1
	else
		if(math.abs(player.x-a.x)<3) then
			if(player.x+2<a.x) then
				b.vx=-5
			elseif(player.x-2>a.x) then
				b.vx=5
			else
				b.vx=0
			end
			a:focus_on_entity(player)
			if(game.interval(2000)) then
				local bullet=world.create_bullet(a,"ukol",1000)
				bullet.userdata.create_time=game.timer
				bullet.userdata.entity=a.id
			end
		end
	end
end
