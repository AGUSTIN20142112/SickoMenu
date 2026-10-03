#include "pch-il2cpp.h"
#include "settings_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "game.h"
#include "achievements.hpp"
#include "DirectX.h"
#include "imgui/imgui_impl_win32.h" // ImGui_ImplWin32_GetDpiScaleForHwnd
#include "theme.hpp" // ApplyTheme

namespace SettingsTab {
	enum Groups {
		General,
		Spoofing,
		Customization,
		Keybinds
	};

	static bool openGeneral = true; //default to general tab group
	static bool openSpoofing = false;
	static bool openCustomization = false;
	static bool openKeybinds = false;

	void CloseOtherGroups(Groups group) {
		openGeneral = group == Groups::General;
		openSpoofing = group == Groups::Spoofing;
		openCustomization = group == Groups::Customization;
		openKeybinds = group == Groups::Keybinds;
	}

	void OpenSubGroup(const std::string& name) {
		if (name == "General") CloseOtherGroups(Groups::General);
		else if (name == "Spoofing" || name == "Suplantar") CloseOtherGroups(Groups::Spoofing);
		else if (name == "Customization" || name == "Personalizar") CloseOtherGroups(Groups::Customization);
		else if (name == "Keybinds" || name == "Atajos") CloseOtherGroups(Groups::Keybinds);
	}
	void CheckKeybindEdit(bool hotKey) {
		State.KeybindsBeingEdited = State.KeybindsBeingEdited || hotKey;
	}

	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###SettingsButtons", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		if (TabGroup("General", openGeneral)) {
			CloseOtherGroups(Groups::General);
		}
		ImGui::SameLine();
		if (TabGroup("Suplantar", openSpoofing)) {
			CloseOtherGroups(Groups::Spoofing);
		}
		ImGui::SameLine();
		if (TabGroup("Personalizar", openCustomization)) {
			CloseOtherGroups(Groups::Customization);
		}
		ImGui::SameLine();
		if (TabGroup("Atajos", openKeybinds)) {
			CloseOtherGroups(Groups::Keybinds);
		}

		ImGui::BeginChild("###Settings", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		if (openGeneral) {
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			if (ToggleButton("Permitir Atajos mientras Escribes", &State.KeybindsWhileChatting)) {
				State.Save();
			}

			if (ToggleButton("Permitir Clics a traves del Menu", &State.ClickThroughMenuUI)) {
				State.Save();
			}

			if (ToggleButton("Mostrar Menu al Iniciar", &State.ShowMenuOnStartup)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Aviso de Panico", &State.PanicWarning)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Comandos Extra", &State.ExtraCommands)) {
				State.Save();
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Escribe \"/help\" en el chat para ver todos los comandos disponibles.");
			}
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			// sorry to anyone trying to read this code it is pretty messy
#pragma region New config menu, needs fixing
			/*
			std::vector<std::string> CONFIGS = GetAllConfigs();
			CONFIGS.push_back("[New]");
			CONFIGS.push_back("[Delete]");

			std::vector<const char*> CONFIGS_CHAR;

			for (const std::string& str : CONFIGS) {
				char* ch = new char[str.size() + 1];
				std::copy(str.begin(), str.end(), ch);
				ch[str.size()] = '\0';
				CONFIGS_CHAR.push_back(ch);
			}

			bool isNewConfig = CONFIGS.size() == 1;
			bool isDelete = false;

			int& selectedConfigInt = State.selectedConfigInt;
			std::string selectedConfig = CONFIGS[selectedConfigInt];

			if (CustomListBoxInt("Configs", &selectedConfigInt, CONFIGS_CHAR), 100 * State.dpiScale, ImVec4(0,0,0,0), ImGuiComboFlags_NoArrowButton) {
				isNewConfig = selectedConfigInt == CONFIGS.size() - 2;
				isDelete = selectedConfigInt == CONFIGS.size() - 1;
				if (!isNewConfig && !isDelete) State.selectedConfig = CONFIGS[selectedConfigInt];
				State.Save();
				State.Load();
			}

			if (isNewConfig || isDelete) {
				InputString("Name", &State.selectedConfig);
				if (isNewConfig && (AnimatedButton(CheckConfigExists(State.selectedConfig) ? "Overwrite" : "Save"))) {
					State.Save();
					CONFIGS = GetAllConfigs();

					selectedConfigInt = std::distance(CONFIGS.begin(), std::find(CONFIGS.begin(), CONFIGS.end(), State.selectedConfig));
				}

				if (isDelete && CheckConfigExists(State.selectedConfig)) {
					if (AnimatedButton("Delete")) {
						selectedConfigInt--;
						State.Delete();
						CONFIGS = GetAllConfigs();
						if (selectedConfigInt < 0) selectedConfigInt = 0;
					}
				}
			}*/
#pragma endregion

			InputString("Nombre de Configuracion", &State.selectedConfig);

			if (CheckConfigExists(State.selectedConfig) && AnimatedButton("Cargar Config"))
			{
				State.SaveConfig();
				State.Load();
				State.Save(); //actually save the selected config
			}
			if (CheckConfigExists(State.selectedConfig)) ImGui::SameLine();
			if (AnimatedButton("Guardar Config"))
			{
				State.Save();
			}
			if (!CheckConfigExists(State.selectedConfig)) {
				ImGui::Text("¡Configuracion no encontrada!");
				ImGui::SameLine();
			}

			/*if (ToggleButton("Adjust by DPI", &State.AdjustByDPI)) {
				if (!State.AdjustByDPI) {
					State.dpiScale = 1.0f;
				}
				else {
					State.dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(DirectX::window);
				}
				State.dpiChanged = true;
				State.Save();
			}*/

			/*static const std::vector<const char*> DPI_SCALING_LEVEL = {"50%", "60%", "70%", "80%", "90%", "100%", "110%", "120%", "130%", "140%", "150%", "160%", "170%", "180%", "190%", "200%", "210%", "220%", "230%", "240%", "250%", "260%", "270%", "280%", "290%", "300%"};
			
			int scaleIndex = (int(std::clamp(State.dpiScale, 0.5f, 3.0f) * 100.0f) - 50) / 5;
			if (CustomListBoxInt("Menu Scale", &scaleIndex, DPI_SCALING_LEVEL, 100 * State.dpiScale)) {
				State.dpiScale = (scaleIndex * 10 + 50) / 100.0f;
				State.dpiChanged = true;
			}*/

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ImGui::InputInt("FPS", &State.GameFPS)) {
				State.GameFPS = std::clamp(State.GameFPS, 10, 100000);
			}

			ImGui::Dummy(ImVec2(1, 1) * State.dpiScale);

			if (ToggleButton("Auto-Salir por Bajos FPS", &State.LeaveDueLFPS)) {
				State.Save();
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(80 * State.dpiScale);
			ImGui::InputInt("FPS Minimos", &State.minFpsThreshold);
			if (State.minFpsThreshold < 0)
				State.minFpsThreshold = 0;
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

#ifdef _DEBUG
			if (ToggleButton("Mostrar Pestaña de Depuración", &State.showDebugTab)) {
				State.Save();
			}
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
#endif
			/*if (!IsHost() && !State.SafeMode && !IsNameValid(State.userName)) {
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
				if (InputString("Username", &State.userName)) State.Save();
				ImGui::PopStyleColor();
			}
			else */InputString("Nombre de Usuario", &State.userName);

			if (!IsNameValid(State.userName) && !IsHost() && State.SafeMode) {
				if (State.userName == "")
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre vacio detectado por anticheat.");
				if (State.userName.length() > (size_t)10)
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre demasiado largo (max 10).");
				else if (!IsNameValid(State.userName))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre contiene caracteres invalidos.");
				else
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre detectado por anticheat.");
			}

			// you can only join a lobby if you have the same name as what your requested name is, when trying to join it
			if (IsNameValid(State.userName) && (!State.SafeMode ||
				State.CurrentScene == "MatchMaking" || State.CurrentScene == "MainMenu" || State.CurrentScene == "Tutorial" || State.CurrentScene == "HowToPlay")) {
				if (AnimatedButton("Fijar como Nombre de Cuenta")) {
					SetPlayerName(State.userName);
					LOG_INFO("Successfully set account name to \"" + State.userName + "\"");
				}
			}

			if (/*IsNameValid(State.userName) || IsHost() || */!State.SafeMode) {
				if (IsInGame() || IsInLobby()) ImGui::SameLine();
				if ((IsInGame() || IsInLobby()) && AnimatedButton("Fijar Nombre")) {
					if (IsInGame())
						State.rpcQueue.push(new RpcSetName(State.userName));
					else if (IsInLobby())
						State.lobbyRpcQueue.push(new RpcSetName(State.userName));
					LOG_INFO("Nombre en el juego establecido exitosamente a \"" + State.userName + "\"");
				}
				if (IsInGame() || IsInLobby()) ImGui::SameLine();
				if (ToggleButton("Fijar Nombre Automaticamente", &State.SetName)) {
					State.Save();
				}
			}

			InputString("Codigo Personalizado", &State.customCode);

			if (ToggleButton("Reemplazar Codigo en Modo Streamer", &State.HideCode)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Codigo de Sala RGB", &State.RgbLobbyCode)) {
				State.Save();
			}

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			static float timer = 0.0f;
			static bool CosmeticsNotification = false;

			if (ToggleButton("Desbloquear Cosmeticos", &State.UnlockCosmetics)) {
				State.Save();
				CosmeticsNotification = true;
				timer = static_cast<float>(ImGui::GetTime());
			}

			if (CosmeticsNotification) {
				float currentTime = static_cast<float>(ImGui::GetTime());
				if (currentTime - timer < 5.0f) {
					ImGui::SameLine();
					if (State.UnlockCosmetics)
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "¡Cosmeticos Desbloqueados!");
					else
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡Cosmeticos Bloqueados!");
				}
				else {
					CosmeticsNotification = false;
				}
			}

			if (Achievements::IsSupported())
			{
				ImGui::SameLine();
				if (AnimatedButton("Desbloquear Todos los Logros"))
					State.unlockAllAchievements = true;
			}

			if (ToggleButton("Permitir que otros vean que usas SickoMenu", &State.ModDetection)) State.Save();
			/*ImGui::SameLine();
			if (CustomListBoxInt(" ", &State.BroadcastedMod, MODS, 100.f * State.dpiScale)) State.Save();*/
		}
		if (openSpoofing) {
			/*if (ToggleButton("Spoof Guest Account", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (State.SpoofGuestAccount) {
				ImGui::SameLine();
				if (ToggleButton("Use Custom Guest Friend Code", &State.UseNewFriendCode)) {
					State.Save();
				}
				if (State.UseNewFriendCode) {
					if (InputString("Guest Friend Code", &State.NewFriendCode)) {
						State.Save();
					}
					ImGui::Text("Guest friend code should be <= 10 characters long and cannot have a hashtag.");
				}
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Pro Tip: You can bypass the free chat restriction using a space after your custom friend");
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "code!");
			}*/
			/*if (AnimatedButton("Force Login as Guest")) {
				State.ForceLoginAsGuest = true;
			}*/
			if (ToggleButton("Suplantar Cuenta de Invitado (Solo Chat Rapido)", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (ToggleButton("Codigo de Amigo Personalizado (Solo Cuenta Nueva/Invitado)", &State.UseNewFriendCode)) {
				State.Save();
			}
			if (State.UseNewFriendCode) {
				ImGui::SetNextItemWidth(150 * State.dpiScale); // Adjust the width of the input box

				bool isFriendCodeValid = State.NewFriendCode.find(" ") == std::string::npos && State.NewFriendCode.length() <= 10;
				if (!isFriendCodeValid) ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
				InputString("Codigo de Amigo", &State.NewFriendCode);
				if (!isFriendCodeValid) ImGui::PopStyleColor();

				auto friendCodeValidText = "El codigo debe tener <= 10 caracteres y no tener espacios.\nDejalo vacio para generar uno aleatorio.";
				if (isFriendCodeValid) ImGui::Text(friendCodeValidText);
				else ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), friendCodeValidText);

				if (State.SpoofGuestAccount)
					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Nota: Otros jugadores no pueden ver tu codigo de invitado.");
			}
			if (ToggleButton("Suplantar Nivel", &State.SpoofLevel)) {
				State.Save();
			}
			if (State.SpoofLevel) {
				ImGui::SameLine();
				ImGui::SetNextItemWidth(120.f * State.dpiScale);
				ImGui::InputInt("Nivel", &State.FakeLevel);

				if (State.SafeMode && (State.FakeLevel <= 0 || State.FakeLevel > 100001))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "El nivel debe estar entre 0 y 100001 para no ser detectado.");
			}

			if (ToggleButton("Suplantar Plataforma", &State.SpoofPlatform)) {
				State.Save();
			}
			if (State.SpoofPlatform) {
				ImGui::SameLine();
				if (CustomListBoxIntColored("Plataforma", &State.FakePlatform, PLATFORMS, 225.0F, ImVec4(1.f, 1.f, 1.f, 0.f), 0, " ", PLATFORM_NAMES_COLOR, IM_ARRAYSIZE(PLATFORM_NAMES_COLOR)))
					State.Save();
			}

			if (State.FakePlatform == 9) {
				if (ToggleButton("Suplantar ID de PlayStation (PSN)", &State.SpoofPsnId)) {
					State.Save();
				}
				if (State.SpoofPsnId)
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(150 * State.dpiScale);
					ImGui::InputScalar("ID PSN Falsa", ImGuiDataType_U64, &State.FakePsnId);

					if (AnimatedButton("ID PSN Aleatoria")) {
						GeneratePlatformId();
					}
				}
			}

			if (State.FakePlatform == 8) {
				if (ToggleButton("Suplantar ID de Xbox", &State.SpoofXboxId)) {
					State.Save();
				}
				if (State.SpoofXboxId)
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(150 * State.dpiScale);
					ImGui::InputScalar("ID Xbox Falsa", ImGuiDataType_U64, &State.FakeXboxId);

					if (AnimatedButton("ID Xbox Aleatoria")) {
						GeneratePlatformId();
					}
				}
			}

			if (ToggleButton("Suplantar Nombre de Plataforma", &State.SpoofPlName)) {
				State.Save();
			}
			if (State.SpoofPlName)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150 * State.dpiScale);
				InputString("Nombre de Plataforma", &State.FakePlName);
			}

			static bool dhaWarnState = false;

			if (!dhaWarnState && ToggleButton("Reducir Anticheat al ser Host (Modo +25)", &State.DisableHostAnticheat)) {
				if (State.DisableHostAnticheat) {
					dhaWarnState = true;
					State.DisableHostAnticheat = false;
				}
				else State.Save();

				if (!State.DisableHostAnticheat && State.BattleRoyale) {
					State.BattleRoyale = false;
					State.GameMode = 0;
				}
			}

			if (dhaWarnState) {
				BoldText("Advertencia", ImVec4(1.f, 0.f, 0.f, 1.f));
				ImGui::Text("Al activar Reducir Anticheat al ser Host (Modo +25),");
				ImGui::Text("tu sala SOLO podra ser vista por otros usuarios con mods,");
				ImGui::Text("o con el codigo de la sala.");
				ImGui::Text(" ");
				ImGui::Text("Tu sala tendra anticheat reducido para todos,");
				ImGui::Text("permitiendo acciones que normalmente serian bloqueadas.");
				ImGui::Text(" ");
				ImGui::Text("¿Estas seguro de activarlo?");

				if (ColoredButton(ImVec4(0.f, 1.f, 0.f, 1.f), "Si")) {
					dhaWarnState = false;
					State.DisableHostAnticheat = true;
					State.Save();
				}
				ImGui::SameLine();
				if (ColoredButton(ImVec4(1.f, 0.f, 0.f, 1.f), "No")) {
					dhaWarnState = false;
				}
			}

			static bool cssWarnState = false;

			if (!cssWarnState && ToggleButton("Ajustes de Servidor Personalizado", &State.UseCustomServer)) {
				if (State.UseCustomServer) {
					cssWarnState = true;
					State.UseCustomServer = false;
				}
				else {
					State.Save();
				}
			}

			if (cssWarnState) {
				BoldText("Advertencia", ImVec4(1.f, 0.f, 0.f, 1.f));
				ImGui::Text("Fuerza a todas las salas nuevas a conectarse a una IP y puerto especificos.");
				ImGui::Text("¿Estas seguro de activarlo?");

				if (ColoredButton(ImVec4(0.f, 1.f, 0.f, 1.f), "Si")) {
					cssWarnState = false;
					State.UseCustomServer = true;
					State.Save();
				}
				ImGui::SameLine();
				if (ColoredButton(ImVec4(1.f, 0.f, 0.f, 1.f), "No")) {
					cssWarnState = false;
				}
			}

			if (State.UseCustomServer) {
				static char ipBuffer[128] = "";

				if (ipBuffer[0] == '\0' && !State.CustomServerIp.empty()) {
					strncpy_s(ipBuffer, State.CustomServerIp.c_str(), sizeof(ipBuffer));
				}

				if (ImGui::InputText("Server IP", ipBuffer, sizeof(ipBuffer))) {
					State.CustomServerIp = ipBuffer;
				}

				int port = static_cast<int>(State.CustomServerPort);
				if (ImGui::InputInt("Server Port", &port, 1, 100)) {
					if (port < 1) port = 1;
					if (port > 65535) port = 65535;
					State.CustomServerPort = static_cast<uint16_t>(port);
				}
			}

			ImGui::Spacing();

			if (ToggleButton("Forzar DTLS", &State.ForceDTLS)) {
				State.Save();
			}
			/*if (State.DisableHostAnticheat) {
				BoldText("Warning (+25 Mode)", ImVec4(1.f, 0.f, 0.f, 1.f));
				BoldText("With this option enabled, you can only find public lobbies with +25 enabled.");
				BoldText("You may not find any public lobbies in the game listing due to this.");
				BoldText("This is intended behaviour, do NOT report it as a bug.");
			}*/
			/*if (ToggleButton("Spoof Among Us Version", &State.SpoofAUVersion))
				State.Save();
			if (State.SpoofAUVersion) {
				ImGui::SameLine();
				if (CustomListBoxInt("Version", &State.FakeAUVersion, AUVERSIONS))
					State.Save();
			}*/
		}

		if (openCustomization) {
			if (ToggleButton("Ocultar Marca de Agua", &State.HideWatermark)) {
				State.Save();
				ReloadCurrentSceneIfNeeded();
			}
			ImGui::SameLine();
			if (ToggleButton("Ocultar Sello de Mod", &State.HideModStamp)) {
				State.Save();
			}

			if (!State.GradientMenuTheme) {
				if (ImGui::ColorEdit3("Color del Menu", (float*)&State.MenuThemeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			else {
				if (ImGui::ColorEdit3("Color Degradado 1", (float*)&State.MenuGradientColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
				ImGui::SameLine();
				if (ImGui::ColorEdit3("Color Degradado 2", (float*)&State.MenuGradientColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			ImGui::SameLine();
			if (ToggleButton("Tema Degradado", &State.GradientMenuTheme))
				State.Save();

			if (ToggleButton("Fondo acorde al Tema", &State.MatchBackgroundWithTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Tema del Menu RGB", &State.RgbMenuTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (AnimatedButton("Restablecer Color"))
			{
				State.MenuThemeColor = ImVec4(1.f, 0.f, 0.424f, State.MenuThemeColor.w);
				State.GradientMenuTheme = false;
				State.RgbMenuTheme = false;
				State.MatchBackgroundWithTheme = false;
				State.Save();
			}

			SteppedSliderFloat("Opacidad", (float*)&State.MenuThemeColor.w, 0.1f, 1.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ToggleButton("Tema Oscuro del Juego", &State.DarkMode)) {
				State.Save();
				State.MIG_ThemeChanged = true;
				ReloadCurrentSceneIfNeeded();
			}
			ImGui::SameLine();
			if (ToggleButton("Tema Personalizado del Juego", &State.CustomGameTheme)) {
				State.Save();
				State.MIG_ThemeChanged = true;
			}

			if (State.CustomGameTheme) {
				if (ImGui::ColorEdit3("Color de Fondo", (float*)&State.GameBgColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
					State.MIG_ThemeChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::ColorEdit3("Color de Texto", (float*)&State.GameTextColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
					State.Save();
			}
			if (ToggleButton("Cambiar Fuente de Chat", &State.ChatFont)) {
				State.Save();
			}
			if (State.ChatFont) {
				ImGui::SameLine();
				if (CustomListBoxInt("", &State.ChatFontType, FONTS, 160.f * State.dpiScale)) {
					State.Save();
				}
			}

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			ImGui::Text("Mostrar junto al Ping:");
			if (ToggleButton("Mostrar FPS", &State.ShowFps)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("Mostrar Hora", &State.ShowTime)) {
				State.Save();
			}

			if (State.ShowTime) {
				static int hours = State.TimeOffsetMinutes / 60, minutes = State.TimeOffsetMinutes % 60;
				static int timeOffsetChoice = State.NegativeTimeOffset;
				ImGui::Text("Diferencia Horaria (UTC)");
				ImGui::SameLine();
				if (CustomListBoxInt("  ", &timeOffsetChoice, TIME_OFFSETS, 20.f * State.dpiScale)) {
					State.NegativeTimeOffset = (bool)timeOffsetChoice;
					State.Save();
				}
				ImGui::SameLine();
				// there can only be 1440 minutes in a day
				ImGui::SetNextItemWidth(60 * State.dpiScale);
				if (ImGui::InputInt("  :", &hours)) {
					hours = std::clamp(hours, 0, 23);
					State.TimeOffsetMinutes = hours * 60 + minutes;
					hours = State.TimeOffsetMinutes / 60, minutes = State.TimeOffsetMinutes % 60;
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(60 * State.dpiScale);
				if (ImGui::InputInt("   ", &minutes)) {
					minutes = std::clamp(minutes, 0, 59);
					State.TimeOffsetMinutes = hours * 60 + minutes;
					hours = State.TimeOffsetMinutes / 60, minutes = State.TimeOffsetMinutes % 60;
				}
			}

			if (ImGui::CollapsingHeader("Formato de Hora")) {
				ImGui::Text(("Vista Previa: " +
					GetTimeString(State.UseLeadingZeroForHours, State.ShowSeconds)).c_str());

				if (ToggleButton("Formato 12 Horas", &State.Use12HourFormat)) State.Save();

				if (ToggleButton("Cero a la Izquierda en Horas", &State.UseLeadingZeroForHours)) State.Save();

				if (ToggleButton("Mostrar Segundos", &State.ShowSeconds)) State.Save();

				if (State.Use12HourFormat) {
					ImGui::SetNextItemWidth(100 * State.dpiScale);
					InputString("AM", &State.AmString);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100 * State.dpiScale);
					InputString("PM", &State.PmString);
				}
			}

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			if (ImGui::CollapsingHeader("Interfaz (GUI)")) {
				if (ToggleButton("Modo Claro", &State.LightMode)) State.Save();
				ImGui::SameLine();
				if (ToggleButton("Mostrar Bordes de Interfaz", &State.ShowUiBorders)) State.Save();

				ImGui::SetNextItemWidth(50 * State.dpiScale);
				if (ImGui::InputFloat("Escala del Menu", &State.dpiScale)) {
					State.dpiScale = std::clamp(State.dpiScale, 0.5f, 3.f);
					State.dpiChanged = true;
				}
				if (ToggleButton("Desactivar Animaciones", &State.DisableAnimations))
					State.Save();
				if (ImGui::InputFloat("Velocidad de Animacion", &State.AnimationSpeed)) {
					if (State.AnimationSpeed <= 0) State.AnimationSpeed = 1.f;
				}
				SteppedSliderFloat("Radio de Redondeo", &State.RoundingRadiusMultiplier, 0.f, 2.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

				ImGui::Text("Alineacion de Notificaciones:");
				ImGui::SameLine();
				static int toastsOnTopSelector = (int)State.ToastsOnTop;
				if (CustomListBoxInt(" ", &toastsOnTopSelector, { "Abajo", "Arriba" }, 50.f * State.dpiScale)) {
					State.ToastsOnTop = (bool)toastsOnTopSelector;
					State.Save();
				}
				ImGui::SameLine();
				if (CustomListBoxInt("  ", &State.ToastPositionX, { "Izquierda", "Centro", "Derecha" }, 60.f * State.dpiScale)) {
					State.Save();
				}

				ImGui::SetNextItemWidth(60.f * State.dpiScale);
				if (ImGui::InputInt("Max Notificaciones", &State.MaxToasts)) {
					State.MaxToasts = std::clamp(State.MaxToasts, 1, 6);
				}

				SteppedSliderFloat("Duracion de Notificaciones", &State.ToastMaxDuration, 0.5f, 10.0f, 0.5f, "%.1f s", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
			}

			if (ImGui::CollapsingHeader("Colores de Roles")) {
				ImGui::ColorEdit4("Tripulante", (float*)&State.CrewmateColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Cientifico", (float*)&State.ScientistColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Ingeniero", (float*)&State.EngineerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Bocina", (float*)&State.NoisemakerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Rastreador", (float*)&State.TrackerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Detective", (float*)&State.DetectiveColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Juez", (float*)&State.JudgeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Impostor", (float*)&State.ImpostorColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Metamorfo", (float*)&State.ShapeshifterColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Fantasma", (float*)&State.PhantomColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Vibora", (float*)&State.ViperColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Fantasma Impostor", (float*)&State.ImpostorGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Angel Guardian", (float*)&State.GuardianAngelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Influencer", (float*)&State.InfluencerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Fantasma Tripulante", (float*)&State.CrewmateGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("Restablecer Colores de Roles")) {
					State.CrewmateGhostColor = ImVec4(0.482f, 0.741f, 0.580f, 0.5f);
					State.CrewmateColor = ImVec4(0.071f, 0.984f, 0.996f, 1.f);
					State.EngineerColor = ImVec4(0.043f, 0.506f, 0.780f, 1.f);
					State.GuardianAngelColor = ImVec4(0.129f, 0.737f, 0.988f, 0.5f);
					State.ScientistColor = ImVec4(0.318f, 0.067f, 0.835f, 1.f);
					State.ImpostorColor = ImVec4(0.898f, 0.118f, 0.267f, 1.f);
					State.ShapeshifterColor = ImVec4(0.839f, 0.60f, 0.227f, 1.f);
					State.ImpostorGhostColor = ImVec4(0.671f, 0.384f, 0.553f, 0.5f);
					State.NoisemakerColor = ImVec4(0.212f, 0.898f, 0.180f, 1.f);
					State.TrackerColor = ImVec4(0.737f, 0.235f, 0.863f, 1.f);
					State.PhantomColor = ImVec4(0.443f, 0.235f, 0.075f, 1.f);
					State.DetectiveColor = ImVec4(0.718f, 0.678f, 0.980f, 1.f);
					State.ViperColor = ImVec4(1.0f, 0.937f, 0.455f, 1.f);
					State.JudgeColor = ImVec4(0.0f, 0.588f, 0.204f, 1.f);
					State.InfluencerColor = ImVec4(0.486f, 0.f, 0.596f, 0.5f);
					State.Save();
				}
			}

			if (ImGui::CollapsingHeader("Otros Colores")) {
				ImGui::ColorEdit4("Anfitrion de la Sala", (float*)&State.HostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("ID de Jugador", (float*)&State.PlayerIdColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Nivel de Jugador", (float*)&State.LevelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Plataforma", (float*)&State.PlatformColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Uso de Mod", (float*)&State.ModUsageColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Verificador de Nombres", (float*)&State.NameCheckerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("Codigo de Amigo", (float*)&State.FriendCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("Nombres Marcados", (float*)&State.DaterNamesColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("Codigo de Sala", (float*)&State.LobbyCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("Antiguedad de Sala", (float*)&State.AgeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("Restablecer Otros Colores")) {
					State.HostColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.PlayerIdColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LevelColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.PlatformColor = ImVec4(0.73f, 0.f, 1.f, 1.f);
					State.ModUsageColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.NameCheckerColor = ImVec4(1.f, 0.67f, 0.f, 1.f);
					State.FriendCodeColor = ImVec4(0.2f, 0.6f, 1.f, 1.f);
					State.DaterNamesColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LobbyCodeColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.AgeColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.Save();
				}
			}
		}

		if (openKeybinds) {
			State.KeybindsBeingEdited = false; // This should not stay on permanently

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Menu));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Mostrar/Ocultar Menu");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Console));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Mostrar/Ocultar Consola");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Radar));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Mostrar/Ocultar Radar");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Replay));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Mostrar/Ocultar Repeticion");

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ChatAlwaysActive));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Alternar Boton de Chat Siempre Visible");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ReadGhostMessages));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Leer Mensajes de Fantasmas");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Sicko));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Modo Panico");

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Leave_Game));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Salir de la Partida");

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Hud));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Activar/Desactivar HUD");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Freecam));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Camara Libre");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Zoom));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Zoom");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Noclip));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Atravesar Paredes (NoClip)");

			/*ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Autokill));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Autokill");*/

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Reset_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Restablecer Apariencia");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Randomize_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Confundir Apariencia");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Repair_Sabotage));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Reparar Todos los Sabotajes");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_All_Doors));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Cerrar Todas las Puertas");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_Current_Room_Door));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Cerrar Puerta de la Sala Actual");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Complete_Tasks));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Completar Todas las Tareas");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Cancel_Start));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("Cancelar Inicio de Partida");
		}
		ImGui::EndChild();
		ImGui::EndChild();
	}
}
