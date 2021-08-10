function mv25.init(w,e)
	w.dx=0.4
	w.dy=0.05
	w.bullet1="762"
	w.texture="weapon/mv25.png"
end
function mv25.fire2(w,e)
	if(not game.interval(100)) then return 0 end
	local bullet=world.create_bullet(e,"bullet_762",100)
	bullet.userdata.create_time=game.time
	bullet.userdata.entity=e.id
	play("shot.flac",e.x,e.y)
	return 1
end
