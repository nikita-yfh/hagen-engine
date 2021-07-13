Light={lights={};bgr=Color(0,0,0,0),draw=true}
Light.init=function()
	Light.tex=graphics.create_texture(graphics.display.w,graphics.display.h)
	local shader=Shader("common.vert","light.frag")
	shader:add_tex("light"):set_tex(Light.tex);
	graphics.set_texture_shader("_all",shader)
	Light.shader=shader
end
Light.add=function(x,y,texture,radius)
	if(not Light.tex) then Light.init() end
	graphics.preload(texture)
	local l={}
	l.x=x
	l.y=y
	l.texture=texture
	l.radius=radius
	l.angle=0
	l.enabled=true
	table.insert(Light.lights,l)
end
Light.set_background=function(l)
	Light.bgr=l
end
Light.enable=function()
	graphics.set_texture_shader("_all",Light.shader)
end
Light.disable=function()
	graphics.unset_texture_shader("_all")
end
Light.add_body=function(body,texture,radius)
	if(not Light.tex) then Light.init() end
	graphics.preload(texture)
	local l={}
	l.body=body.id
	l.texture=texture
	l.radius=radius
	l.enabled=true
	Light.lights[body.id]=l
end
Light.render=function()
	graphics.set_target(Light.tex)
	graphics.clear()
	if(Light.draw) then
		graphics.set_target(Light.tex)
		graphics.rect(Light.bgr,0,0,graphics.display.w,graphics.display.h,1)
		for index,l in pairs(Light.lights) do
			local tex=graphics.texture(l.texture)
			local scale=l.radius/tex.w*game.camera.zoom*2
			if(l.enabled) then
				if(l.body) then
					local b=body(l.body)
					if(not b) then Light.lights[index]=nil end
					graphics.blit(tex,
								graphics.drawx(b.x),
								graphics.drawy(b.y),
								b.angle/math.pi*180,scale,scale)
				else
					graphics.blit(tex,
								graphics.drawx(l.x),
								graphics.drawy(l.y),
								l.angle/math.pi*180,scale,scale)
				end
			end
		end
		--[[graphics.set_target(Light.shadow)
		graphics.clear()
		local a=player
		local xp,yp=false,false
		for i=0,math.pi*2+math.pi/160,math.pi/160 do
			local x,y=a.x+math.cos(i)*20,a.y+math.sin(i)*20
			world.raycast(a.x,a.y,x,y,
				function(fixture,px,py,nx,ny,f)
					if(world.who(fixture.body) or fixture.mode~=0) then return -1 end
					x,y=px,py
					return f
				end)
			if(xp and yp) then
				graphics.triangle(Color(255,255,255,255),graphics.drawx(a.x),graphics.drawy(a.y),graphics.drawx(xp),graphics.drawy(yp),graphics.drawx(x),graphics.drawy(y),1)
			end
			xp,yp=x,y
		end]]
	end
	graphics.unset_target()
end
