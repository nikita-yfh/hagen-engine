function module.init(body,ref,subtitles)
	body.userdata.ref=ref
	body.userdata.subtitles=subtitles
end
function module.update(body)
	if(world.eb_all_collide(player,body)) then
		modules[body.userdata.ref]=true
		subtitles(text.get(body.userdata.subtitles))
		return true
	end
	return false
end
