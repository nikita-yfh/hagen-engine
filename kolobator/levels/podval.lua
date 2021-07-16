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
power_state=false
sleep_sub_time=0
sleep_timer=0
sleep_stage=0
poweron=false
break_lamp=false
function level.update()
	camera_focus=""
	game.camera.center(player.x,9.25)
	if(not end_timer and player and world.eb_all_collide(player,body("_end"))) then
		if(power_state) then
			end_timer=game.time+subtitles(text.get("podval/normal_end"))
		else
			end_timer=game.time+subtitles(text.get("podval/dark_end"))
		end
		player.userdata.move=false
		game.interface=false
	end
	if(end_timer and game.time>end_timer) then
		loadlevel("ground1")
	end
	if(not power_state) then
		if(world.eb_all_collide(player,body("power"))) then
			local b=body("power")
			text.add_tip(b.x,b.y-0.3,text.get("podval/poweron_tip"))
			if(game.press_key("action")) then
				Light.lights["power"].enabled=false
				Light.lights["lamp"].enabled=true
				power_state=true
				graphics.preload("poweron.png")
				b:fixture("b").texture="poweron.png"
				subtitles(text.get("podval/poweron"))
				sound.play("poweron.flac")
				poweron=true
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
	end
	if(sub_time1>=0 and game.press_key("action") or (sleep_stage==4 and game.time>sleep_sub_time)) then
		sub_time1=-1
		Light.draw=true
		player.userdata.move=true
		game.interface=true
		if(sleep_stage==4) then
			subtitles(text.get("podval/wake_up2"))
		else
			subtitles(text.get("podval/wake_up1"))
		end
	end
	if(world.eb_all_collide(player,body("t2"))) then
		if(body("knife")) then
			subtitles(text.get("podval/knife_need"))
		else
			subtitles(text.get("podval/boxes"))
		end
		world.destroy_body(body("t2"))
	end
	if(world.eb_all_collide(player,body("t3")) and body("knife")) then
		subtitles(text.get("podval/knife_not_get"))
		world.destroy_body(body("t3"))
	end
	if(not break_lamp and not body("lamp")) then
		subtitles(text.get("podval/cut_wire"))
		break_lamp=true
	end
	subtitles_trigger(body("t1"),text.get("podval/knife_see"))
	if(world.eb_all_collide(entity("player"),body("knife"))) then
		text.add_tip(body("knife").x,body("knife").y-1,text.get("knife_tip"))
		if(game.press_key("action")) then
			weapon["knife"]=true
			entity("player"):set_weapon("knife")
			world.destroy_body(body("knife"))
			subtitles(text.get("podval/knife_get"))
		end
	end
end
