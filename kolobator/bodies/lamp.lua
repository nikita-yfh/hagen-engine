function lamp.init(body)
	Light.add_body(body,"light/1.png",50)
	body.userdata.health=1
	body.userdata.killfunc=function(body)
		play("lamp.flac",body.x,body.y)
		Light.lights[body.id]=nil
	end
end
function lamp.update(body)
	breakable.update(body)
end
