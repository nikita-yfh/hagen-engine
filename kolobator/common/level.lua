function Level.init()
    --local shader=Shader("common.vert","rain.frag")
	--shader:add_tex("tex1"):set("noise.png");
	--shader:add_float("time"):set(game.time);
	--shader:add_vec2("resolution"):set(graphics.display.w,graphics.display.h);
    --graphics.set_texture_shader("_all",shader)
    
    local shader=Shader("common.vert","water.frag")
	shader:add_tex("tex1"):set("diplacement.png");
    graphics.set_texture_shader("water.png",shader)
    
end
function Level.update()
    --local shader=graphics.get_shader("_all")
	--shader:add_float("time"):set(game.time);
    local shader=graphics.get_shader("water.png")
	shader:add_float("time"):set(game.time);
end