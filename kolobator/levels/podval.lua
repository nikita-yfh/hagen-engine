function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
	tx=graphics.create_texture(100,100)
	game.interface=false
	graphics.set_mask(Color(0,0,0,255))
	time=subtitles(text.get("podval/sleep"))
end
local state=false
function level.update()
	player.userdata.move=state
	camera_focus=""
	if(player) then
		game.camera.center(player.x,9.25)
	end
	level_trigger(body("_end"),"ground1")
end
