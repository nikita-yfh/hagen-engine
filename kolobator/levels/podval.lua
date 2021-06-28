function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
end
function level.update()
	camera_focus=""
	if(entity("player")) then
		game.camera.center(player.x,9.25)
	end
end