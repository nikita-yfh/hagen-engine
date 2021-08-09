function level.newgame()
	weapon["knife"]=true
	if(player) then player:set_weapon("knife") end
end
function level.init()
	subtitles(text.get("ground1/beginning"))
end
local nora_fall_c=0
local nora_fall_begin=false
function level.update()
	subtitles_trigger(body("hole_falling"),text.get("ground1/hole_falling"))
	subtitles_trigger(body("hole_exitfind"),text.get("ground1/hole_exitfind"))
	subtitles_trigger(body("sor_find"),text.get("ground1/secretoldroom"))
	
	if(entity("player")) then
		if(world.eb_all_collide(player,body("nora_falling_begin"))) then
			nora_fall_begin=true
		elseif(nora_fall_begin and world.eb_all_collide(player,body("nora_falling_end"))) then
			nora_fall_begin=false
			nora_fall_c=nora_fall_c+1
			if(nora_fall_c<=3) then
				subtitles(text.get("ground1/nora_falling"..nora_fall_c))
				if(nora_fall_c==3) then
					body("nora_block"):fixture("nora_block_f").mode=0
				end
			end 
		end
	end
end