#include "pch-il2cpp.h"
#include "radar_tab.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "utility.h"

namespace RadarTab {
	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Radar", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
		if (ToggleButton("Mostrar Radar", &State.ShowRadar)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("Mostrar Cuerpos Muertos", &State.ShowRadar_DeadBodies)) {
			State.Save();
		}
		if (ToggleButton("Mostrar Fantasmas", &State.ShowRadar_Ghosts)) {
			State.Save();
		}
		if (ToggleButton("Clic Derecho para Teletransportar", &State.ShowRadar_RightClickTP)) {
			State.Save();
		}
		if (ToggleButton("(Shift + Clic Izquierdo) Cerrar Puerta", &State.ShowRadar_ShiftLeftClickClosesRoomDoor)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("Ocultar Radar en Reuniones", &State.HideRadar_During_Meetings)) {
			State.Save();
		}
		if (ToggleButton("Dibujar Iconos de Jugadores", &State.RadarDrawIcons)) {
			State.Save();
		}
		/*if (State.RadarDrawIcons && State.RevealRoles) {
			ImGui::SameLine();
			if (ToggleButton("Show Role Color on Visor", &State.RadarVisorRoleColor)) {
				State.Save();
			}
		}*/

		if (ToggleButton("Bloquear Posicion del Radar", &State.LockRadar)) {
			State.Save();
		}
		if (ToggleButton("Mostrar Borde", &State.RadarBorder)) {
			State.Save();
		}
		if (ImGui::ColorEdit4("Color del Radar",
			(float*)&State.SelectedColor,
			ImGuiColorEditFlags__OptionsDefault
			| ImGuiColorEditFlags_NoInputs
			| ImGuiColorEditFlags_AlphaBar
			| ImGuiColorEditFlags_AlphaPreview)) {
			State.Save();
		}

		ImGui::SetNextItemWidth(100.f * State.dpiScale);
		if (ImGui::InputInt("Ancho Extra", &State.RadarExtraWidth)) {
			State.RadarExtraWidth = abs(State.RadarExtraWidth); //prevent negatives
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f * State.dpiScale);
		if (ImGui::InputInt("Alto Extra", &State.RadarExtraHeight)) {
			State.RadarExtraHeight = abs(State.RadarExtraHeight); //prevent negatives
		}

		ImGui::EndChild();
	}
}