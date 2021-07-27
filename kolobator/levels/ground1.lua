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
	if(subtitles_trigger(body("plata_01"),text.get("ground1/art_find"))) then
		modules["mpz"]=true
	end
	if(subtitles_trigger(body("plata_01"),text.get("ground1/art_find"))) then
		modules["mpz"]=true
	end
end
