function portal.init(body,fixture,x,y)
	body.userdata.portal_f=fixture
	body.userdata.portal_x=x
	body.userdata.portal_y=y
	graphics.preload("portal/1.png")
	graphics.preload("portal/2.png")
	graphics.preload("portal/3.png")
	graphics.preload("portal/4.png")
end
function portal.update(body)
	local e=world.sb_all_collide(body)
	if(e) then
		e.x=body.userdata.portal_x
		e.y=body.userdata.portal_y
	end
	local fix=body:fixture(body.userdata.portal_f)
	if(game.interval(100)) then
		if(fix.texture=="portal/1.png") then fix.texture="portal/2.png"
		elseif(fix.texture=="portal/2.png") then fix.texture="portal/3.png"
		elseif(fix.texture=="portal/3.png") then fix.texture="portal/4.png"
		else fix.texture="portal/1.png" end
	end
end