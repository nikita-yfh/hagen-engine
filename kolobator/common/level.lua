function level_trigger(body,name)
	print(entity("player"))
	if(entity("player")~=nil and world.eb_all_collide(entity("player"),body)) then
		loadlevel(name)
	end
end
function play(name,x,y)
	if(entity("player")~=nil) then
		sound.play_distance(name,math.sqrt(math.pow(player.x-x,2)+math.pow(player.y-y,2)))
	else
		sound.play(name)
	end
end
function Level.init()
    shader=Shader("v1.vert","rain.frag")
	shader:add_tex("tex1"):set("noise.png");
	shader:add_float("time"):set(game.time);
	shader:add_vec2("resolution"):set(graphics.display.w,graphics.display.h);
    graphics.set_texture_shader("ground.png",shader)
end