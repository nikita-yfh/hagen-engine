function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
	Light.set_background(Color(255,255,255,50))
	Light.lights["lamp"].enabled=false
end
local power_state=false
function level.update()
	camera_focus=""
	game.camera.center(player.x,9.25)
	level_trigger(body("_end"),"ground1")
	if(not power_state) then
		if(world.eb_all_collide(player,body("power"))) then
			local b=body("power")
			text.add_tip(b.x,b.y-0.3,text.get("poweron"))
			if(game.press_key("action")) then
				Light.lights["power"].enabled=false
				Light.lights["lamp"].enabled=true
				power_state=true
				b:set_texture("b","poweron")
			end
		end
	end
	Light.render()
end
