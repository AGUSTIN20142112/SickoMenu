#include "pch-il2cpp.h"
#include "esp_tab.h"
#include "game.h"
#include "state.hpp"
#include "utility.h"
#include "gui-helpers.hpp"

namespace EspTab {

	void Render() {
		bool changed = false;
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###ESP", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		changed |= ToggleButton("Mostrar ESP", &State.ShowEsp);

		changed |= ToggleButton("Mostrar Jugadores", &State.ShowEsp_Players);
		changed |= ToggleButton("Mostrar Fantasmas", &State.ShowEsp_Ghosts);
		changed |= ToggleButton("Mostrar Cuerpos Muertos", &State.ShowEsp_DeadBodies);

		ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

		changed |= ToggleButton("Sombras en Lineas y Texto", &State.ShowEsp_LineTextShadows);

		ImGui::SetNextItemWidth(100.f * State.dpiScale);
		if (ImGui::InputFloat("Grosor de Linea", &State.ShowEsp_LineThickness))
			State.ShowEsp_LineThickness = std::clamp(State.ShowEsp_LineThickness, 1.f, 5.f);

		ImGui::SetNextItemWidth(100.f * State.dpiScale);
		if (ImGui::InputFloat("Tamano del Texto", &State.ShowEsp_TextSize))
			State.ShowEsp_TextSize = std::clamp(State.ShowEsp_TextSize, 1.f, 1.5f);

		ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

		changed |= ToggleButton("Ocultar en Reuniones", &State.HideEsp_During_Meetings);

		changed |= ToggleButton("Mostrar Cajas", &State.ShowEsp_Box);
		changed |= ToggleButton("Mostrar Lineas (Tracers)", &State.ShowEsp_Tracers);
		changed |= ToggleButton("Mostrar Distancias", &State.ShowEsp_Distance);
		//better esp (from noobuild) coming v3.1
		changed |= ToggleButton("Color segun Rol en vez de Jugador", &State.ShowEsp_RoleBased);

		changed |= ToggleButton("Mostrar Tripulantes", &State.ShowEsp_Crew);
		ImGui::SameLine();
		changed |= ToggleButton("Mostrar Impostores", &State.ShowEsp_Imp);

		ImGui::EndChild();
		if (changed) {
			State.Save();
		}
	}
}