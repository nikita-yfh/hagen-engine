function knife.init(w,e)
	w.dy=0.02
	w.dx=0.2
	w.texture="weapon/knife.png"
end
function knife.fire1(w,e)
	if(e.userdata.knife_timer~=nil and game.time-e.userdata.knife_timer < math.pi*50) then
		return 0
	end
	e.userdata.knife_timer=game.time
	local length=0.3
	local body=false
	local point={}
	local x,y=e.x+math.cos(e.weapon.angle)*length,e.y+math.sin(e.weapon.angle)*length
	world.raycast(e.x,e.y,x,y,
			function(fixture,px,py,nx,ny,fraction)
				local e2=world.who(fixture.body)
				if(e2 and e2.id==e.id) then return -1 end
				body=fixture.body
				point.x,point.y=px,py
				return fraction
			end)
	if(body) then crash(body,point.x,point.y,5) end
	return 0
end

function knife.update(w,e)
	if(e.userdata.knife_timer~=nil) then
		local val=(game.time-e.userdata.knife_timer)/100;
		w.dx=0.2-math.sin(math.min(math.pi,val*1.5))*0.2
	end
end
