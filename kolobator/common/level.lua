function Level.init()
    --local shader=Shader("common.vert","rain.frag")
	--shader:add_tex("tex1"):set("noise.png");
	--shader:add_float("time"):set(game.time);
	--shader:add_vec2("resolution"):set(graphics.display.w,graphics.display.h);
    --graphics.set_texture_shader("_all",shader)
    
    local shader=Shader("common.vert","water.frag")
	shader:add_tex("tex1"):set("diplacement.png");
    graphics.set_texture_shader("water.png",shader)
    graphics.texture_scale=0.5
    graphics.weapon_scale=0.5
    graphics.effect_scale=0.5
    game.camera.zoom=200
    world.position_iterations=10
    world.velocity_iterations=10
end
function Level.update()
    --local shader=graphics.get_shader("_all")
	--shader:add_float("time"):set(game.time);
    local shader=graphics.get_shader("water.png")
	shader:add_float("time"):set(game.time);
end
function Level.newgame()
	if(entity("player")) then player:set_weapon("knife") end
	bullet("9mm").count=30
	bullet("9mm").max=30
	bullet("762").count=50
	bullet("762").max=50
	bullet("grenade").count=6
	bullet("grenade").max=6
end