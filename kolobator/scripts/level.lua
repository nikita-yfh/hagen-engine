function Level.init()
	local shader=graphics.Shader("common.vert","water.frag") --шейдер воды
	shader:add_tex("tex1"):set("diplacement.png");
	graphics.set_texture_shader("water.png",shader)
	graphics.texture_scale=0.5
	graphics.weapon_scale=0.5
	graphics.effect_scale=0.5
	game.interface=true
	game.camera.zoom=200
	world.position_iterations=6
	world.velocity_iterations=10
	if(player) then
		camera_focus="player" --направляю камеру на ГГ, если он есть
	end
	Light.init()
	Light.bgr=Color(100,100,100,255)
end
function Level.update()
	local shader=graphics.get_shader("water.png")
	shader:add_float("time"):set(game.time);
	Light.render()
end
function Level.newgame()
	weapon={__keep=true;rg6=true}
	bullet("grenade").max=6
	bullet("9mm").max=30
	bullet("762").max=50
	
	bullet("grenade").count=6
	modules={__keep=true;}
end
