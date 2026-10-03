#include "pch-il2cpp.h"
#include "debug_tab.h"
#include "imgui/imgui.h"
#include "state.hpp"
#include "main.h"
#include "game.h"
#include "profiler.h"
#include "logger.h"
#include <iostream>
#include <sstream>
#include "gui-helpers.hpp"
#include "toasts.hpp"

namespace DebugTab {

	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Debug", ImVec2(500, 0) * State.dpiScale, true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
#ifndef _VERSION
		if (AnimatedButton("Descargar DLL"))
		{
			SetEvent(hUnloadEvent);
		}
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
#endif
		ToggleButton("Activar Eliminación de Oclusión", &State.OcclusionCulling);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

		if (AnimatedButton("Forzar Carga de Ajustes"))
		{
			State.Load();
		}
		if (AnimatedButton("Forzar Guardado de Ajustes"))
		{
			State.Save();
		}
		if (AnimatedButton("Limpiar Colas RPC"))
		{
			State.rpcQueue = std::queue<RPCInterface*>();
			State.lobbyRpcQueue = std::queue<RPCInterface*>();
		}

		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

		if (ToggleButton("Registrar Mensajes de Depuración de Unity", &State.ShowUnityLogs)) State.Save();
		if (ToggleButton("Registrar Mensajes de Depuración de Hooks", &State.ShowHookLogs)) State.Save();

		static int toastCount = 0;
		if (AnimatedButton("Mostrar Toast de Ejemplo")) {
			Toasts::AddToast("SickoMenu", std::format("¡Hola desde una notificación toast! ({})", toastCount).c_str());
			toastCount++;
		}
		ImGui::SameLine();
		if (AnimatedButton("Mostrar Toast de Ejemplo (Mensaje Largo)")) {
			Toasts::AddToast("SickoMenu", std::format("El software está hecho para ser utilizado. A menudo viene con alguna forma de interfaz de usuario. Tú, como usuario, debes explorar esta interfaz para familiarizarte con el software instalado en tu equipo... pista: probablemente será un menú llamado 'exclusiones' o 'lista blanca'... ({})", toastCount).c_str());
			toastCount++;
		}

		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

		if (ImGui::CollapsingHeader("Experimentos##debug")) {
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Estas funciones están en desarrollo y pueden fallar en cualquier momento.");
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Úsalas bajo tu propio riesgo.");
			if (ToggleButton("Sistema de Puntos (Solo para Anfitrión)", &State.TournamentMode)) State.Save();
			if (ToggleButton("Modo Día de los Inocentes", &State.AprilFoolsMode)) State.Save();
			/*static float timer = 0.0f;
			static bool SafeModeNotification = false;*/
			static bool safeModeWarnState = false;

			if (!safeModeWarnState && ToggleButton("Modo Seguro", &State.SafeMode)) {
				if (!State.SafeMode) {
					safeModeWarnState = true;
					State.SafeMode = true;
				}
				/*SafeModeNotification = true;
				timer = static_cast<float>(ImGui::GetTime());*/
			}

			if (safeModeWarnState) {
				BoldText("Advertencia", ImVec4(1.f, 0.f, 0.f, 1.f));
				ImGui::Text("Al desactivar el Modo Seguro, puedes desbloquear funciones");
				ImGui::Text("que usualmente son detectadas por el antitrampas.");
				ImGui::Text(" ");
				ImGui::Text("Sin embargo, DEBES asegurarte de que el anfitrión de la sala tenga");
				ImGui::Text("un antitrampas reducido (autoridad de host), para que las otras funciones funcionen.");
				ImGui::Text(" ");
				ImGui::Text("De lo contrario, ¡serás expulsado o baneado por el antitrampas!");
				ImGui::Text("NOTA: Los desarrolladores NO se harán responsables de esto.");
				ImGui::Text(" ");
				ImGui::Text("¿Estás seguro de que deseas desactivarlo?");

				if (ColoredButton(ImVec4(0.f, 1.f, 0.f, 1.f), "Sí")) {
					safeModeWarnState = false;
					State.SafeMode = false;
				}
				ImGui::SameLine();
				if (ColoredButton(ImVec4(1.f, 0.f, 0.f, 1.f), "No")) {
					safeModeWarnState = false;
				}
			}

			/*if (SafeModeNotification) {
				float currentTime = static_cast<float>(ImGui::GetTime());

				if (currentTime - timer < 5.0f) {
					ImGui::SameLine();
					if (State.SafeMode)
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Safe Mode is Enabled!");
					else
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Safe Mode is Disabled! (The likelihood of getting banned increases)");
				}
				else {
					SafeModeNotification = false;
				}
			}*/

			// ImGui::Text("Keep safe mode on in official servers (NA, Europe, Asia) to prevent anticheat detection!");
		}

		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

		if (ImGui::CollapsingHeader("Repetición##debug"))
		{
			synchronized(Replay::replayEventMutex) {
				size_t numWalkPoints = 0;
				for (const auto& pair : State.replayWalkPolylineByPlayer) {
					numWalkPoints += pair.second.pendingPoints.size() + pair.second.simplifiedPoints.size();
				}
				ImGui::Text("Puntos de Ruta: %d", numWalkPoints);
				ImGui::Text("Eventos de Repetición en Vivo: %d", State.liveReplayEvents.size());
				ImGui::Text("Eventos de Consola en Vivo: %d", State.liveConsoleEvents.size());
			}

			ImGui::Text("Inicio de Partida Repetición: %s", std::format("{:%OH:%OM:%OS}", State.MatchStart).c_str());
			ImGui::Text("Momento Actual de Repetición: %s", std::format("{:%OH:%OM:%OS}", State.MatchCurrent).c_str());
			ImGui::Text("Repetición en Vivo: %s", std::format("{:%OH:%OM:%OS}", std::chrono::system_clock::now()).c_str());
			ImGui::Text("Repetición Es En Vivo: %s", (State.Replay_IsLive) ? "Sí" : "No");
			ImGui::Text("Repetición Reproduciendo: %s", (State.Replay_IsPlaying) ? "Sí" : "No");

			if (AnimatedButton("Re-simplificar polilíneas (ver consola)"))
			{
				SYNCHRONIZED(Replay::replayEventMutex);
				for (auto& playerPolylinePair : State.replayWalkPolylineByPlayer)
				{
					std::vector<ImVec2> resimplifiedPoints;
					std::vector<std::chrono::system_clock::time_point> resimplifiedTimeStamps;
					Replay::WalkEvent_LineData& plrLineData = playerPolylinePair.second;
					size_t numOldSimpPoints = plrLineData.simplifiedPoints.size();
					DoPolylineSimplification(plrLineData.simplifiedPoints, plrLineData.simplifiedTimeStamps, resimplifiedPoints, resimplifiedTimeStamps, 50.f, false);
					STREAM_DEBUG("Player[" << playerPolylinePair.first << "]: Re-simplification could reduce " << numOldSimpPoints << " points to " << resimplifiedPoints.size());
				}
			}
		}

		if (ImGui::CollapsingHeader("Colores##debug"))
		{
			il2cpp::Array colArr = app::Palette__TypeInfo->static_fields->PlayerColors;
			auto colArr_raw = colArr.begin();
			size_t length = colArr.size();
			for (size_t i = 0; i < length; i++)
			{
				const app::Color32& col = colArr_raw[i];
				const ImVec4& conv_col = AmongUsColorToImVec4(col);
				static constexpr std::array COLORS = { "Rojo", "Azul", "Verde", "Rosa", "Naranja", "Amarillo", "Negro", "Blanco", "Morado", "Marrón", "Cian", "Lima", "Granate", "Rosa Claro", "Plátano", "Gris", "Bronceado", "Coral", "Verde Fuerte" };
				ImGui::TextColored(conv_col, "%s [%d]: (%d, %d, %d, %d)", COLORS.at(i), i, col.r, col.g, col.b, col.a);
			}
		}

		if (ImGui::CollapsingHeader("Rendimiento##debug"))
		{
			if (AnimatedButton("Limpiar Estadísticas"))
			{
				Profiler::ClearStats();
			}

			std::stringstream statStream;
			Profiler::AppendStatStringStream("WalkEventCreation", statStream);
			Profiler::AppendStatStringStream("ReplayRender", statStream);
			Profiler::AppendStatStringStream("ReplayPolyline", statStream);
			Profiler::AppendStatStringStream("PolylineSimplification", statStream);
			Profiler::AppendStatStringStream("ReplayPlayerIcons", statStream);
			Profiler::AppendStatStringStream("ReplayEventIcons", statStream);
			// NOTE:
			// can also just do this to dump all stats, but i like doing them individually so i can control the order better:
			// Profiler::WriteStatsToStream(statStream);

			ImGui::TextUnformatted(statStream.str().c_str());
		}

		ImGui::Text(std::format("Escena Activa: {}", State.CurrentScene).c_str());

		ImGui::Text(std::format("FPS Actuales: {}", GetFps()).c_str());

		ImGui::EndChild();
	}
}