light={lights={}}
light.init=function()
	light.tex=graphics.create_texture(graphics.display.w,graphics.display.h)
	local shader=Shader("common.vert","light.frag")
	shader:add_tex("light"):set_tex(light.tex);
	graphics.set_texture_shader("_all",shader)
	light.shader=shader
end
light.add=function(x,y,texture,radius)
	graphics.preload(texture)
	local l={}
	l.x=x
	l.y=y
	l.texture=texture
	l.radius=radius
	table.insert(light.lights,l)
end
light.render=function()
	graphics.set_target(light.tex)
	graphics.clear()
	for _,l in pairs(light.lights) do
		local tex=graphics.texture(l.texture)
		local scale=l.radius/tex.w*game.camera.zoom*2
		graphics.blit(tex,
						graphics.drawx(l.x)-tex.w/2*scale,
						graphics.drawy(l.y)-tex.h/2*scale,
						0,scale,scale)
	end
	graphics.unset_target()
end
