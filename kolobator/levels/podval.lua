function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
	Light.enable()
	Light.set_background(Color(255,255,255,50))
	Light.lights["lamp"].enabled=false
	Light.draw=false
	player.userdata.move=false
	game.interface=false
	sub_time1=game.time+subtitles(text.get("podval/sleep"))
end
local power_state=false
local sleep_sub_time=0
local sleep_timer=0
local sleep_stage=0
function level.update()
	camera_focus=""
	game.camera.center(player.x,9.25)
	level_trigger(body("_end"),"ground1")
	if(not power_state) then
		if(world.eb_all_collide(player,body("power"))) then
			local b=body("power")
			text.add_tip(b.x,b.y-0.3,text.get("podval/poweron_tip"))
			if(game.press_key("action")) then
				Light.lights["power"].enabled=false
				Light.lights["lamp"].enabled=true
				power_state=true
				b:set_texture("b","poweron")
			end
		end
	end
	if(sub_time1>=0 and game.time>sub_time1) then
		text.add_tip(player.x,player.y,text.get("podval/wake_tip"))
		if(sleep_timer==0)then sleep_timer=game.time end
		if(sleep_stage<4 and game.time>sleep_timer+15000) then
			sleep_timer=game.time
			sleep_stage=sleep_stage+1
			sleep_sub_time=game.time+subtitles(text.get("podval/no_wake"..sleep_stage))
		end
		if(game.press_key("action") or (sleep_stage==4 and game.time>sleep_sub_time)) then
			sub_time1=-1
			Light.draw=true
			player.userdata.move=true
			game.interface=true
			if(sleep_time==4) then
				subtitles(text.get("podval/wake_up2"))
			else
				subtitles(text.get("podval/wake_up1"))
			end
		end
	end
end
