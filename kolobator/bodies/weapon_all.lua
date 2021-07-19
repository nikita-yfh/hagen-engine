function weapon_all.init(b,id,tip,sound)
	b.userdata.weaponid=id
	b.userdata.weapontip=tip
	b.userdata.weaponsound=sound
end
function weapon_all.update(b)
	if(world.eb_all_collide(entity("player"),b)) then
		if(b.userdata.weapontip) then
			text.add_tip(b.x,b.y-1,text.get(b.userdata.weapontip))
		end
		if(game.press_key("action")) then
			if(b.userdata.weaponsound) then
				play(b.userdata.weaponsound,b.x,b.y)
			end
			weapon[b.userdata.weaponid]=true
			entity("player"):set_weapon(b.userdata.weaponid)
			return true
		end
	end
end
