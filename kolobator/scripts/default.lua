function Level:init()
	print("Loading 'default' level...")
end
function Level:update()
	if(game.key("1")) then
		game.camera.zoom=game.camera.zoom+1
	elseif(game.key("2")) then
		game.camera.zoom=game.camera.zoom-1
	elseif(game.key("3")) then
		graphics.show_textures=true
	elseif(game.key("4")) then
		graphics.show_textures=false
	end
end
