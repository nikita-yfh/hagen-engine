function Entity.update(e)
	if(camera_focus==e.id) then
		game.camera.center(e.x,e.y)
	end
end
