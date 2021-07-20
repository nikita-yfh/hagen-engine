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

	if(world.eb_all_collide(player,body("nora2"))) then
		body("nora2"):fixture("fg2").layer=-128
		body("nora2"):fixture("fg2").texture="ground_dark.png"
	else
		body("nora2"):fixture("fg2").layer=127
		body("nora2"):fixture("fg2").texture="ground.png"
	end
	if(world.eb_all_collide(player,body("nora3"))) then
		body("nora3"):fixture("fg3").layer=-128
		body("nora3"):fixture("fg3").texture="ground_dark.png"
	else
		body("nora3"):fixture("fg3").layer=127
		body("nora3"):fixture("fg3").texture="ground.png"
	end
end
