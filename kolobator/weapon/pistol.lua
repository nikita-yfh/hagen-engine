function pistol.init(w,e)
	w.dx=0.1
	w.dy=0.1
	w.bullet1="9mm"
	w.texture="weapon/pistol.png"
end
function pistol.fire2(w,e)
	if(not game.interval(17)) then   return 0 end 
	local bullet=world.create_bullet(e,"bullet_9mm",100)
	bullet.userdata.create_time=game.time
	bullet.userdata.entity=e.id
	play("shot.flac",e.x,e.y)
	return 0
end
