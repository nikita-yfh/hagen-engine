function w_knife.init(b,help)
	b.userdata.help=help
end
function w_knife.update(b)
	if(world.eb_all_collide(entity("player"),b)) then
		if(b.userdata.help) then
			text.add_tip(b.x,b.y-1,text.get("knife_tip"))
		end
		if(game.press_key("action")) then
			entity("player").userdata.weapons["knife"]=true
			entity("player"):set_weapon("knife")
			return 1
		end
	end
end
