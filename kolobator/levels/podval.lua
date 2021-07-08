function level.init()
	light.init()
	light.add(11,8.25,"light1.png",50)
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
end
local state=false
function level.update()
	camera_focus=""
	if(player) then
		game.camera.center(player.x,9.25)
	end
	level_trigger(body("_end"),"ground1")
	light.render()
end
