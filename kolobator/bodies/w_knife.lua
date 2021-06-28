function w_knife.update(b)
	if(world.eb_all_collide(entity("player"),b)) then
		text.add_tip(b.x,b.y-1,text.get("put_knife"))
		if(game.press_key("action")) then
			entity("player").userdata.weapons["knife"]=true
			entity("player"):set_weapon("knife")
			return 1
		end
	end
end