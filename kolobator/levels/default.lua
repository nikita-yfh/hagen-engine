function Level:init()
	print("Loading 'default' level...")
end
--e
function Level:update()
	if(game.key("1")) then
		game.camera.zoom=game.camera.zoom+1
	end
	if(game.key("2")) then
		game.camera.zoom=game.camera.zoom-1
	end
end
