function level.newgame()
	weapon["knife"]=true
	if(player) then player:set_weapon("knife") end
	graphics.preload("ground_dark.png")
end
function level.update()
	if(world.eb_all_collide(player,body("nora"))) then
		body("nora"):fixture("fg").layer=-128
		body("nora"):fixture("fg").texture="ground_dark.png"
	else
		body("nora"):fixture("fg").layer=127
		body("nora"):fixture("fg").texture="ground.png"
	end
	if(world.eb_all_collide(entity("player"),body("pistol"))) then
		if(game.press_key("action")) then
			weapon["pistol"]=true
			entity("player"):set_weapon("pistol")
			world.destroy_body(body("pistol"))
		end
	end
end
