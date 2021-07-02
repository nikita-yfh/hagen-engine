function level.init()
	game.camera.zoom = math.floor(graphics.display.h / 3.5)
	tx=graphics.create_texture(100,100)
	graphics.bind_texture(tx,"tex")
end
function level.update()
	graphics.framebuffer(tx)
	local b=false
	if(math.random()>0.5) then b=true end
	graphics.rect(math.random(0,100),math.random(0,100),math.random(0,100),math.random(0,100),Color(math.random(0,255),math.random(0,255),math.random(0,255),255),b)
	graphics.framebuffer(nil)
	camera_focus=""
	if(entity("player")) then
		game.camera.center(player.x,9.25)
	end
end