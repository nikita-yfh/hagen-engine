function nora.init(body,fixture)
	body.userdata.fixture=fixture
	graphics.preload("ground_dark.png")
	graphics.preload("ground.png")
end
function nora.update(body)
	if(world.eb_all_collide(player,body)) then
		body:fixture(body.userdata.fixture).layer=-128
		body:fixture(body.userdata.fixture).texture="ground_dark.png"
	else
		body:fixture(body.userdata.fixture).layer=127
		body:fixture(body.userdata.fixture).texture="ground.png"
	end
end
