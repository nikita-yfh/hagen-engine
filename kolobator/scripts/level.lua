function Level.init()
	local shader=Shader("common.vert","water.frag") --шейдер воды
	shader:add_tex("tex1"):set("diplacement.png");
    graphics.set_texture_shader("water.png",shader)
    graphics.texture_scale=0.5
    graphics.weapon_scale=0.5
    graphics.effect_scale=0.5
    game.camera.zoom=200
    if(system.os=="Android") then
		world.position_iterations=18
		world.velocity_iterations=18
	else
		world.position_iterations=100
		world.velocity_iterations=100
	end
	if(player) then
		camera_focus="player" --направляю камеру на ГГ, если он есть
	end
	Light.init()
	Light.disable()
end
function Level.update()
    local shader=graphics.get_shader("water.png")
	shader:add_float("time"):set(game.time);
	Light.render()
	if(game.press_key("left")) then print("P") end
	if(game.key("left")) then print("\tK") end
	if(game.release_key("left")) then print("\t\tR") end
end
function Level.newgame()
	player.userdata.weapons={}
end
