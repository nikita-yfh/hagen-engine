function knife.init(w,e)
	w.dy=0.02
	w.dx=0.2
	w.texture="weapon/knife.png"
end
function knife.fire1(w,e)
	if(e.userdata.knife_timer~=nil and game.timer-e.userdata.knife_timer < math.pi*100) then 
		return 0
	end
	e.userdata.knife_timer=game.timer
	local mas={}
	mas[e.id]=1
	world.raycast_callbacks(e.x,e.y,1.5,function(en,d)
		if(mas[en.id]==nil) then
			graphics.effect("blood",en.x,en.y)
			en:harm(5)
			mas[en.id]=true
		end
	end,nil)
	return 0
end

function knife.update(w,e)
	if(e.userdata.knife_timer~=nil) then
		local val=(game.timer-e.userdata.knife_timer)/100;
		w.dx=0.2-math.sin(math.min(math.pi,val))*0.2
	end
end
