function level.init()
	light.init()
	light.add(player.x,player.y,"light1.png",5)
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
end
local state=false
function level.update()
	camera_focus=""
	if(player) then
		game.camera.center(player.x,9.25)
	end
	level_trigger(body("_end"),"ground1")
	light.lights[1].x=player.x
	light.lights[1].y=player.y
	light.render()
end
