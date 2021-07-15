Pause={shown=false;width=0;}
function Pause.draw()
	if(not Pause.shown) then return end
	ImGui.SetNextWindowPosCenter(ImGui.Cond.FirstUseEver)
	ImGui.Begin("Pause",0,ImGui.WindowFlags.NoTitleBar+
						ImGui.WindowFlags.NoCollapse+
						ImGui.WindowFlags.NoMove+
						ImGui.WindowFlags.NoResize+
						ImGui.WindowFlags.NoFocusOnAppearing)
	local w=Pause.width
	if(ImGui.Button(text.get("pause/continue"),w,0)) then
		Pause.close()
	end
	ImGui.Button(text.get("pause/save_game"),w,0)
	ImGui.Button(text.get("pause/load_game"),w,0)
	ImGui.Button(text.get("pause/settings"),w,0)
	if(ImGui.Button(text.get("pause/main_menu"),w,0)) then
		ImGui.OpenPopup(text.get("pause/main_menu_title"))
	end
	if(ImGui.Button(text.get("pause/exit_game"),w,0)) then
		ImGui.OpenPopup(text.get("pause/exit_game_title"))
	end
	Pause.width=ImGui.GetWindowContentRegionWidth()
	if(ImGui.BeginPopupModal(text.get("pause/main_menu_title"), 0, ImGui.WindowFlags.AlwaysAutoResize)) then
		ImGui.Text(text.get("pause/main_menu_text"))
		ImGui.Separator()
		if(ImGui.Button(text.get("common/ok"))) then
			ImGui.CloseCurrentPopup();
			Pause.close()
		end
		ImGui.SameLine();
		if (ImGui.Button(text.get("common/cancel"))) then
			ImGui.CloseCurrentPopup();
		end
		ImGui.EndPopup();
	end
	if (ImGui.BeginPopupModal(text.get("pause/exit_game_title"), nil, ImGui.WindowFlags.AlwaysAutoResize)) then
		ImGui.Text(text.get("pause/exit_game_text"))
		ImGui.Separator()
		if(Button(text.get("common/ok"))) then
			ImGui.CloseCurrentPopup();
			Pause.close()
			system.exit()
		end
		SameLine();
		if (Imgui.Button(text.get("common/cancel"))) then
			CloseCurrentPopup();
		end
		EndPopup();
	end
	ImGui.End()
end
function Pause.close()
	Pause.shown=false
end
function Pause.open()
	Pause.shown=true
end
