function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
end
local state=false
function level.update()
	camera_focus=""
	if(player) then
		game.camera.center(player.x,9.25)
	end
	level_trigger(body("_end"),"ground1")
	Light.render()
end
