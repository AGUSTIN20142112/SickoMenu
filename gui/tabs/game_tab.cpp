#include "pch-il2cpp.h"
#include "game_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "logger.h"

static std::string strToLower(std::string str) {
    std::string new_str = "";
    for (auto i : str) {
        new_str += char(std::tolower(i));
    }
    return new_str;
}

namespace GameTab {
    enum Groups {
        General,
        Chat,
        Anticheat,
        Utils,
        History,
        Options
    };

    static bool openGeneral = true;
    static bool openChat = false;
    static bool openAnticheat = false;
    static bool openUtils = false;
    static bool openHistory = false;
    static bool openOptions = false;

    void CloseOtherGroups(Groups group) {
        openGeneral = group == Groups::General;
        openChat = group == Groups::Chat;
        openAnticheat = group == Groups::Anticheat;
        openUtils = group == Groups::Utils;
        openHistory = group == Groups::History;
        openOptions = group == Groups::Options;
    }

    void OpenSubGroup(const std::string& name) {
        if (name == "General") CloseOtherGroups(Groups::General);
        else if (name == "Chat") CloseOtherGroups(Groups::Chat);
        else if (name == "Anticheat" || name == "Antitrampas") CloseOtherGroups(Groups::Anticheat);
        else if (name == "Utils" || name == "Utilidades") CloseOtherGroups(Groups::Utils);
        else if (name == "History" || name == "Historial") CloseOtherGroups(Groups::History);
        else if ((name == "Options" || name == "Opciones") && (GameOptions().HasOptions() && (IsInGame() || IsInLobby()))) CloseOtherGroups(Groups::Options);
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###GameButtons", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("General", openGeneral)) {
            CloseOtherGroups(Groups::General);
        }
        ImGui::SameLine();
        if (TabGroup("Chat", openChat)) {
            CloseOtherGroups(Groups::Chat);
        }
        ImGui::SameLine();
        if (TabGroup("Antitrampas", openAnticheat)) {
            CloseOtherGroups(Groups::Anticheat);
        }
        ImGui::SameLine();
        if (TabGroup("Utilidades", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }
        ImGui::SameLine();
        if (TabGroup("Historial", openHistory)) {
            CloseOtherGroups(Groups::History);
        }

        if (GameOptions().HasOptions() && (IsInGame() || IsInLobby())) {
            ImGui::SameLine();
            if (TabGroup("Opciones", openOptions)) {
                CloseOtherGroups(Groups::Options);
            }
        }

        enum WarnViewType {
            WarnView_List = 0,
            WarnView_Manual,
            WarnView_COUNT
        };

        static int selectedWarnView = 0;
        const char* warnViewModes[WarnView_COUNT] = {
            "Vista de Lista",
            "Advertencia Manual"
        };

        ImGui::BeginChild("###Game", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (openGeneral) {
            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);
            if (SteppedSliderFloat("Multiplicador de Velocidad", &State.PlayerSpeed, 0.f, 10.f, 0.05f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevPlayerSpeed = State.PlayerSpeed;
            }
            if (SteppedSliderFloat("Distancia de Asesinato", &State.KillDistance, 0.f, 20.f, 0.1f, "%.1f m", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevKillDistance = State.KillDistance;
            }
            /*if (GameOptions().GetGameMode() == GameModes__Enum::Normal) {
                if (CustomListBoxInt("Task Bar Updates", &State.TaskBarUpdates, TASKBARUPDATES, 225 * State.dpiScale))
                    State.PrevTaskBarUpdates = State.TaskBarUpdates;
            }*/
            /*if (ToggleButton("No Ability Cooldown", &State.NoAbilityCD)) {
                State.Save();
            }
            ImGui::SameLine();*/
            if (ToggleButton("Multiplicar Velocidad", &State.MultiplySpeed)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Modificar Distancia de Asesinato", &State.ModifyKillDistance)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (IsHost() || !State.SafeMode) {
                CustomListBoxIntColored(" ", &State.SelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale, ImVec4(1.f, 1.f, 1.f, 0.f), 0, "", COLOR_NAMES_COLOR, IM_ARRAYSIZE(COLOR_NAMES_COLOR));
            }
            else {
                if (State.SelectedColorId >= (int)COLORS.size()) State.SelectedColorId = 0;
                CustomListBoxIntColored(" ", &State.SelectedColorId, COLORS, 85.0f * State.dpiScale, ImVec4(1.f, 1.f, 1.f, 0.f), 0, "", COLOR_NAMES_COLOR, IM_ARRAYSIZE(COLOR_NAMES_COLOR));
            }
            ImGui::SameLine();
            if (AnimatedButton("Color Aleatorio"))
            {
                State.SelectedColorId = GetRandomColorId();
            }

            if (IsInGame() || IsInLobby()) {
                ImGui::SameLine();
                if (AnimatedButton("Fijar Color"))
                {
                    if (IsHost() || !State.SafeMode) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                    }
                    else if (IsColorAvailable(State.SelectedColorId)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcSetColor(State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcSetColor(State.SelectedColorId));
                    }
                }
            }
            ImGui::SameLine();
            if (ToggleButton("Robar Color", &State.SnipeColor)) {
                State.Save();
            }

            if (ToggleButton("Consola", &State.ShowConsole)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Notificaciones de Consola", &State.ShowConsoleEventsAsToasts)) {
                State.Save();
            }

            /*if (ToggleButton("Auto-Join", &State.AutoJoinLobby))
                State.Save();
            ImGui::SameLine();
            if (InputString("Lobby Code", &State.AutoJoinLobbyCode))
                State.Save();

            if (AnimatedButton("Join Lobby")) {
                AmongUsClient_CoJoinOnlineGameFromCode(*Game::pAmongUsClient,
                    GameCode_GameNameToInt(convert_to_string(State.AutoJoinLobbyCode), NULL),
                    NULL);
            }*/


            if ((IsInGame() || IsInLobby()) && AnimatedButton("Restablecer Apariencia"))
            {
                ControlAppearance(false);
            }


            if (IsInGame() && (IsHost() || !State.SafeMode) && AnimatedButton("Asesinar a Todos")) {
                for (auto player : GetAllPlayerControl()) {
                    if (IsInGame() && (IsHost() || !State.SafeMode)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                    }
                    else {
                        if (IsInGame())
                            State.rpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                    }
                }
            }
            if (IsInLobby() && !State.SafeMode) ImGui::SameLine();
            if (IsInLobby() && !State.SafeMode && AnimatedButton("Permitir NoClip a Todos")) {
                for (auto p : GetAllPlayerControl()) {
                    if (p != *Game::pLocalPlayer) State.lobbyRpcQueue.push(new RpcMurderLoop(*Game::pLocalPlayer, p, 1, true));
                }
                State.NoClip = true;
                ShowHudNotification("¡Se permitió NoClip a todos!");
            }
            /*if (IsHost() && (IsInGame() || IsInLobby()) && AnimatedButton("Spawn Dummy")) {
                auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                if (IsInGame()) State.rpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
                if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
            }*/
            if ((IsInGame() || IsInLobby()) && ((IsHost() && IsInGame()) || !State.SafeMode)) {
                ImGui::SameLine();
                if (AnimatedButton(IsHost() ? "Proteger a Todos" : "Proteger Visualmente a Todos")) {
                    for (auto player : GetAllPlayerControl()) {
                        uint8_t colorId = GetPlayerOutfit(GetPlayerData(player))->fields.ColorId;
                        if (IsInGame())
                            State.rpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                    }
                }
            }

            if (IsInGame() && ToggleButton("Desactivar Ventilaciones", &State.DisableVents)) {
                State.Save();
            }
            if (IsInGame()) {
                ImGui::SameLine();
                if (ToggleButton("Pausar Bloqueo de Ventilación al Usarla", &State.PauseVentBlockingWhileVenting)) {
                    State.Save();
                }
            }
            if (IsInGame() && (IsHost() || !State.SafeMode) && ToggleButton("Spam de Reporte", &State.SpamReport)) {
                State.Save();
            }

            if (IsInGame()/* && (IsHost() || !State.SafeMode)*/) {
                std::vector<const char*> allVents;
                switch (State.mapType) {
                case Settings::MapType::Ship:
                    allVents = SHIPVENTS;
                    break;
                case Settings::MapType::Hq:
                    allVents = HQVENTS;
                    break;
                case Settings::MapType::Pb:
                    allVents = PBVENTS;
                    break;
                case Settings::MapType::Airship:
                    allVents = AIRSHIPVENTS;
                    break;
                case Settings::MapType::Fungle:
                    allVents = FUNGLEVENTS;
                    break;
                }
                State.SelectedVentId = std::clamp(State.SelectedVentId, 0, (int)allVents.size() - 1);

                ImGui::SetNextItemWidth(100 * State.dpiScale);
                CustomListBoxInt("Ventilación", &State.SelectedVentId, allVents);
                
                if (AnimatedButton("Teletransportar Todos a Ventilación")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (State.IgnoreVentTpSelf && p == *Game::pLocalPlayer) continue;
                        if (IsHost() || !State.SafeMode)
                            State.rpcQueue.push(new RpcBootFromVent(p, (State.mapType == Settings::MapType::Hq) ? State.SelectedVentId + 1 : State.SelectedVentId)); //MiraHQ vents start from 1 instead of 0
                        else
                            State.rpcQueue.push(new RpcBootFromVentNonHost(p, (State.mapType == Settings::MapType::Hq) ? State.SelectedVentId + 1 : State.SelectedVentId)); //MiraHQ vents start from 1 instead of 0
                    }
                }
                ImGui::SameLine();
                if (AnimatedButton("Teletransportar Todos a Ventilaciones Aleatorias")) {
                    bool isHq = State.mapType == Settings::MapType::Hq;
                    for (auto p : GetAllPlayerControl()) {
                        if (State.IgnoreVentTpSelf && p == *Game::pLocalPlayer) continue;
                        int randomVentId = randi((int)isHq, (int)allVents.size() - (int)(!isHq));

                        if (IsHost() || !State.SafeMode)
                            State.rpcQueue.push(new RpcBootFromVent(p, randomVentId));
                        else
                            State.rpcQueue.push(new RpcBootFromVentNonHost(p, randomVentId));
                    }
                }

                if (ToggleButton("Spam TP Todos a Ventilación", &State.SpamVentTpEveryone)) {
                    if (State.SpamVentTpEveryone) State.SpamVentTpEveryoneRandom = false;
                }
                ImGui::SameLine();
                if (ToggleButton("Spam TP Todos a Ventilaciones Aleatorias", &State.SpamVentTpEveryoneRandom)) {
                    if (State.SpamVentTpEveryoneRandom) State.SpamVentTpEveryone = false;
                }

                if (ToggleButton("Ignorarme (TP de Ventilación)", &State.IgnoreVentTpSelf)) State.Save();

                if (IsInMultiplayerGame() && AnimatedButton("Intentar Banear a Todos")) {
                    State.rpcQueue.push(new AttemptToBan(NULL));
                }

                if (State.mapType == Settings::MapType::Fungle) {
                    if (AnimatedButton("Hacer Subir Tirolesa a Todos (Abajo hacia Arriba)")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (State.IgnoreZiplineSelf && p == *Game::pLocalPlayer) continue;
                            State.rpcQueue.push(new RpcClimbZipline(p, false));
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Hacer Subir Tirolesa a Todos (Arriba hacia Abajo)")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (State.IgnoreZiplineSelf && p == *Game::pLocalPlayer) continue;
                            State.rpcQueue.push(new RpcClimbZipline(p, true));
                        }
                    }

                    ToggleButton("Spam de Tirolesa para Todos", &State.SpamZiplineEveryone);
                    ImGui::SameLine();
                    if (ToggleButton("Ignorarme (Tirolesa)", &State.IgnoreZiplineSelf)) State.Save();
                }
            }

            if ((IsInGame() || (IsInLobby() && State.KillInLobbies)) && (IsHost() || !State.SafeMode)) {
                if (AnimatedButton("Asesinar a Todos los Tripulantes")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (!PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        }
                    }
                }
                ImGui::SameLine();
                if (AnimatedButton("Asesinar a Todos los Impostores")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                        }
                    }
                }
                if (!State.SafeMode) {
                    ImGui::SameLine();
                    if (AnimatedButton("Suicidar Tripulantes")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (!PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Suicidar Impostores")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                }
            }

            if (IsInGame() || IsInLobby()) {
                bool visuals = GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks);
                if (!State.SafeMode && visuals && AnimatedButton("Escanear a Todos")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, true));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, true));
                    }
                }
                if (!State.SafeMode && visuals) ImGui::SameLine();
                if (!State.SafeMode && visuals && AnimatedButton("Detener Escaneo de Todos")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, false));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, false));
                    }
                }
                if (IsInGame() && !State.InMeeting && !State.SafeMode && visuals) ImGui::SameLine();
                if (IsInGame() && !State.InMeeting && AnimatedButton("Expulsar a Todos de las Ventilaciones")) {
                    State.rpcQueue.push(new RpcBootAllVents());
                }
                if ((IsHost() || !State.SafeMode) && State.InMeeting) ImGui::SameLine();
                if ((IsHost() || !State.SafeMode) && State.InMeeting && AnimatedButton("Terminar Reunión")) {
                    State.rpcQueue.push(new RpcEndMeeting());
                    State.InMeeting = false;
                }

                if (!State.SafeMode && !IsHost()) {
                    if (AnimatedButton("Fijar Nombre para Todos")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("Forzar Nombre para Todos", &State.ForceNameForEveryone)) {
                        State.Save();
                    }

                    InputString("Nombre de Usuario", &State.hostUserName);

                    if (AnimatedButton("Fijar Color para Todos")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("Forzar Color para Todos", &State.ForceColorForEveryone)) {
                        State.Save();
                    }

                    if (CustomListBoxIntColored(" ­", &State.HostSelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale, ImVec4(1.f, 1.f, 1.f, 0.f), 0, "", COLOR_NAMES_COLOR, IM_ARRAYSIZE(COLOR_NAMES_COLOR))) State.Save();
                }
            }
        }

        if (openChat) {
            bool msgAllowed = IsChatValid(State.chatMessage);

            if (!msgAllowed) ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
            InputStringMultiline("\n\n\n\n\nMensaje de Chat", &State.chatMessage);
            if (!msgAllowed) ImGui::PopStyleColor();

            if (!State.chatMessage.empty()) {
                if ((IsInGame() || IsInLobby()) && State.ChatCooldown >= 3.f && IsChatValid(State.chatMessage)) {
                    ImGui::SameLine();
                    if (AnimatedButton("Enviar"))
                    {
                        auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                            State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                        if (IsInGame()) State.rpcQueue.push(new RpcSendChat(player, State.chatMessage));
                        else if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSendChat(player, State.chatMessage));
                        State.MessageSent = true;
                    }
                }
                if ((IsInGame() || IsInLobby()) && State.ReadAndSendSickoChat) ImGui::SameLine();
                if (State.ReadAndSendSickoChat && (IsInGame() || IsInLobby()) && AnimatedButton("Enviar SickoChat"))
                {
                    auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                        State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                    if (IsInGame()) {
                        State.rpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                    }
                    else if (IsInLobby()) {
                        State.lobbyRpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                    }
                }
            }

            if (ToggleButton("Spam", &State.ChatSpam))
            {
                if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                State.Save();
            }
            if (((IsHost() && IsInGame()) || !State.SafeMode) && State.ChatSpamMode) ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && State.ChatSpamMode && ToggleButton("Spam por Todos", &State.ChatSpamEveryone))
            {
                State.Save();
            }
            if ((IsHost() && IsInGame()) || !State.SafeMode) {
                if (CustomListBoxInt("Modo de Spam de Chat", &State.ChatSpamMode,
                    { State.SafeMode ? "Con Mensaje (SOLO Auto-Spam)" : "Con Mensaje", "Chat Vacío", State.SafeMode ? "Auto-Mensaje + Chat Vacío" : "Mensaje + Chat Vacío" })) State.Save();
            }

            if (!(IsHost() || !State.SafeMode) && State.chatMessage.size() > 120) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "El mensaje será detectado por el antitrampas.");
            }

            ImGui::Dummy(ImVec2(0, 4) * State.dpiScale);
            if (ImGui::CollapsingHeader("Preajustes de Chat", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Dummy(ImVec2(0, 2) * State.dpiScale);
                static std::string newChatPresetName = "MiPreajuste";
                static int lastRenameIndex = -1;
                auto chatPresetNameTaken = [](const std::string& name, int excludeIndex) {
                    for (size_t i = 0; i < State.ChatPresets.size(); i++) {
                        if ((int)i == excludeIndex) continue;
                        if (State.ChatPresets[i].Name == name) return true;
                    }
                    return false;
                    };

                if (!State.ChatPresets.empty()) {
                    std::vector<const char*> presetNames;
                    for (auto& p : State.ChatPresets) presetNames.push_back(p.Name.c_str());
                    State.SelectedChatPreset = std::clamp(State.SelectedChatPreset, 0, (int)State.ChatPresets.size() - 1);
                    CustomListBoxInt("Preajuste", &State.SelectedChatPreset, presetNames, 200.0f * State.dpiScale, ImVec4(0, 0, 0, 0), 0);
                    auto& selected = State.ChatPresets[State.SelectedChatPreset];

                    if (lastRenameIndex != State.SelectedChatPreset) {
                        newChatPresetName = selected.Name;
                        lastRenameIndex = State.SelectedChatPreset;
                    }

                    ImGui::SameLine();
                    if (AnimatedButton("Aplicar##chatpreset")) {
                        State.chatMessage = selected.Messages.empty() ? "" : selected.Messages[0];
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Actualizar##chatpreset")) {
                        selected.Messages = { State.chatMessage };
                        State.Save();
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar##chatpreset")) {
                        State.ChatPresets.erase(State.ChatPresets.begin() + State.SelectedChatPreset);
                        if (!State.ChatPresets.empty())
                            State.SelectedChatPreset = std::clamp(State.SelectedChatPreset, 0, (int)State.ChatPresets.size() - 1);
                        lastRenameIndex = -1;
                        State.Save();
                    }
                }
                else {
                    ImGui::TextDisabled("No hay preajustes guardados.");
                }

                ImGui::Dummy(ImVec2(0, 4) * State.dpiScale);

                ImGui::SetNextItemWidth(160 * State.dpiScale);
                InputString("##PresetNameInput", &newChatPresetName);
                ImGui::SameLine();
                ImGui::TextUnformatted("Nombre del Preajuste");
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Los nombres de los preajustes no pueden contener espacios para usarse con /cmd (ej. /rules).");
                ImGui::SameLine();
                if (AnimatedButton("Guardar Actual##chatpreset")) {
                    std::string sanitizedName = newChatPresetName;
                    sanitizedName.erase(std::remove(sanitizedName.begin(), sanitizedName.end(), ' '), sanitizedName.end());
                    if (sanitizedName.empty()) sanitizedName = "Preajuste";
                    if (chatPresetNameTaken(sanitizedName, -1)) {
                        ImGui::OpenPopup("ChatPresetNameTaken");
                    }
                    else {
                        Settings::ChatPreset p;
                        p.Name = sanitizedName;
                        p.Messages = { State.chatMessage };
                        State.ChatPresets.push_back(p);
                        State.SelectedChatPreset = (int)State.ChatPresets.size() - 1;
                        lastRenameIndex = State.SelectedChatPreset;
                        State.Save();
                    }
                }
                if (ImGui::BeginPopup("ChatPresetNameTaken")) {
                    ImGui::Text("Ya existe un preajuste con ese nombre.");
                    ImGui::EndPopup();
                }
                if (!State.ChatPresets.empty()) {
                    ImGui::SameLine();
                    if (AnimatedButton("Renombrar##chatpreset")) {
                        std::string sanitizedName = newChatPresetName;
                        sanitizedName.erase(std::remove(sanitizedName.begin(), sanitizedName.end(), ' '), sanitizedName.end());
                        if (!sanitizedName.empty()) {
                            if (chatPresetNameTaken(sanitizedName, State.SelectedChatPreset)) {
                                ImGui::OpenPopup("ChatPresetNameTaken");
                            }
                            else {
                                State.ChatPresets[State.SelectedChatPreset].Name = sanitizedName;
                                State.Save();
                            }
                        }
                    }
                }

                ImGui::Dummy(ImVec2(0, 4) * State.dpiScale);

                // Only single message presets can be included, to prevent the already combined presets from piling up.
                std::vector<int> combinableIndices;
                for (int i = 0; i < (int)State.ChatPresets.size(); i++) {
                    if (State.ChatPresets[i].Messages.size() <= 1) combinableIndices.push_back(i);
                }

                if (combinableIndices.size() >= 2 && ImGui::CollapsingHeader("Combinar Preajustes")) {
                    ImGui::Dummy(ImVec2(0, 2)* State.dpiScale);
                    static std::vector<std::pair<const char*, bool>> combineList;
                    static std::vector<std::string> combineNamesCache;
                    static size_t lastCombinableCount = 0;
                    static std::vector<bool> prevCombineChecked;
                    static std::vector<int> combineSelectionOrder; // saves into combineList in the order the user checked them
                    if (combineList.size() != combinableIndices.size()) {
                        combineNamesCache.clear();
                        for (int idx : combinableIndices) combineNamesCache.push_back(State.ChatPresets[idx].Name);
                        combineList.clear();
                        for (auto& n : combineNamesCache) combineList.push_back({ n.c_str(), false });
                        lastCombinableCount = combinableIndices.size();
                        prevCombineChecked.assign(combineList.size(), false);
                        combineSelectionOrder.clear();
                    }

                    CustomListBoxIntMultiple("Preajustes a Combinar", &combineList, 220.0f * State.dpiScale);

                    if (prevCombineChecked.size() != combineList.size())
                        prevCombineChecked.assign(combineList.size(), false);
                    for (size_t i = 0; i < combineList.size(); i++) {
                        bool nowChecked = combineList[i].second;
                        bool wasChecked = prevCombineChecked[i];
                        if (nowChecked && !wasChecked) {
                            combineSelectionOrder.push_back((int)i);
                        }
                        else if (!nowChecked && wasChecked) {
                            combineSelectionOrder.erase(std::remove(combineSelectionOrder.begin(), combineSelectionOrder.end(), (int)i), combineSelectionOrder.end());
                        }
                        prevCombineChecked[i] = nowChecked;
                    }

                    ImGui::Dummy(ImVec2(0, 4) * State.dpiScale);
                    static std::string combinedPresetName = "PreajusteCombinado";
                    ImGui::SetNextItemWidth(160 * State.dpiScale);
                    InputString("Nombre del Preajuste Combinado", &combinedPresetName);
                    ImGui::SameLine();
                    if (AnimatedButton("Combinar como Nuevo Preajuste") && combineSelectionOrder.size() >= 2) {
                        std::string sanitizedName = combinedPresetName;
                        sanitizedName.erase(std::remove(sanitizedName.begin(), sanitizedName.end(), ' '), sanitizedName.end());
                        if (sanitizedName.empty()) sanitizedName = "PreajusteCombinado";
                        if (chatPresetNameTaken(sanitizedName, -1)) {
                            ImGui::OpenPopup("ChatPresetNameTaken");
                        }
                        else {
                            Settings::ChatPreset combined;
                            combined.Name = sanitizedName;
                            combined.Messages.clear();
                            for (int i : combineSelectionOrder) {
                                if (i >= 0 && i < (int)combinableIndices.size()) {
                                    for (auto& m : State.ChatPresets[combinableIndices[i]].Messages) combined.Messages.push_back(m);
                                }
                            }
                            if (!combined.Messages.empty()) {
                                State.ChatPresets.push_back(combined);
                                State.SelectedChatPreset = (int)State.ChatPresets.size() - 1;
                                combineList.clear();
                                prevCombineChecked.clear();
                                combineSelectionOrder.clear();
                                State.Save();
                            }
                        }
                    }
                }
            }
        }

        if (openAnticheat) {
            if (ToggleButton("Activar Antitrampas (SMAC)", &State.Enable_SMAC)) State.Save();
            ImGui::Dummy(ImVec2(0, 1)* State.dpiScale);
            if (ImGui::CollapsingHeader("Acción al Detectar##smacoverride")) {
                bool changed = false;

                ImGui::TextDisabled("Acción por defecto cuando una detección no esté anulada abajo.");
                changed = changed || CustomListBoxInt("", &State.SMAC_HostPunishment, SMAC_HOST_PUNISHMENTS, 85.0f * State.dpiScale);
                ImGui::SameLine();
                ImGui::Text("Acción de Anfitrión");
                ImGui::SameLine();
                changed = changed || CustomListBoxInt("  ", &State.SMAC_Punishment, SMAC_PUNISHMENTS, 85.0f * State.dpiScale);
                ImGui::SameLine();
                ImGui::Text("Acción Normal");

                ImGui::Dummy(ImVec2(0, 1) * State.dpiScale);
                ImGui::TextDisabled("Anular la acción para una detección específica.");

                static const std::vector<std::pair<const char*, const char*>> SMAC_CATEGORIES = {
                    { "Uso de Trampas Conocidas", "Known Cheat Usage" },
                    { "Uso de SickoMenu", "SickoMenu Usage" },
                    { "Nombres Anormales", "Abnormal Names" },
                    { "Cambio de Color Anormal", "Abnormal Set Color" },
                    { "Cambio de Cosméticos Anormal", "Abnormal Set Cosmetics" },
                    { "Nota de Chat Anormal", "Abnormal Chat Note" },
                    { "Escáner Anormal", "Abnormal Scanner" },
                    { "Animación Anormal", "Abnormal Animation" },
                    { "Asignación de Tareas", "Setting Tasks" },
                    { "Asesinatos Anormales", "Abnormal Murders" },
                    { "Metamorfosis Anormal", "Abnormal Shapeshift" },
                    { "Desaparición Anormal", "Abnormal Vanish" },
                    { "Reuniones/Reportes Anormales", "Abnormal Meetings/Body Reports" },
                    { "Ventilación Anormal", "Abnormal Venting" },
                    { "Chat Anormal", "Abnormal Chat" },
                    { "Completar Tareas Anormal", "Abnormal Task Completion" },
                    { "Sabotajes Anormales", "Abnormal Sabotages" },
                    { "Niveles de Jugador Anormales", "Abnormal Player Levels" },
                    { "Código de Amigo Anormal", "Abnormal Friend Code" },
                    { "Suplantación de Plataforma", "Abnormal Platform" },
                    { "Palabras Bloqueadas", "Blocked Words" },
                    { "Palabras de Inicio Bloqueadas", "Blocked Start Words" },
                    { "Jugadores en Lista Negra", "Blacklisted Players" },
                };
                static int selectedCategory = 0;
                static std::vector<const char*> smacCategoryLabels;
                if (smacCategoryLabels.empty()) {
                    for (auto& cat : SMAC_CATEGORIES) smacCategoryLabels.push_back(cat.first);
                }

                std::string catKey = SMAC_CATEGORIES[selectedCategory].second;

                auto& hostOverrides = State.SMAC_ReasonPunishmentOverrideHost;
                if (hostOverrides.find(catKey) == hostOverrides.end())
                    hostOverrides[catKey] = State.SMAC_HostPunishment;

                auto& overrides = State.SMAC_ReasonPunishmentOverride;
                if (overrides.find(catKey) == overrides.end())
                    overrides[catKey] = State.SMAC_Punishment;

                hostOverrides[catKey] = std::clamp(hostOverrides[catKey], 0, (int)SMAC_HOST_PUNISHMENTS.size() - 1);
                overrides[catKey] = std::clamp(overrides[catKey], 0, (int)SMAC_PUNISHMENTS.size() - 1);

                ImGui::SetNextItemWidth(150.0f * State.dpiScale);
                CustomListBoxInt("Categoría", &selectedCategory, smacCategoryLabels, 150.0f * State.dpiScale);

                changed = changed || CustomListBoxInt(" ", &hostOverrides[catKey], SMAC_HOST_PUNISHMENTS, 85.0f * State.dpiScale);
                ImGui::SameLine();
                ImGui::Text("Anulación de Anfitrión");
                ImGui::SameLine();
                changed = changed || CustomListBoxInt("   ", &overrides[catKey], SMAC_PUNISHMENTS, 85.0f * State.dpiScale);
                ImGui::SameLine();
                ImGui::Text("Anulación Normal");
                if (changed) State.Save();
            }
            ImGui::Dummy(ImVec2(0, 2)* State.dpiScale);
            if (ToggleButton("Añadir Tramposos a Lista Negra", &State.SMAC_AddToBlacklist)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Castigar Lista Negra", &State.SMAC_PunishBlacklist)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Ignorar Lista Blanca", &State.SMAC_IgnoreWhitelist)) State.Save();
            if (State.SMAC_PunishBlacklist) {
                ImGui::Text("Lista Negra");
                if (State.BlacklistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡No hay usuarios en la lista negra!");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d Usuarios en Lista Negra)", State.BlacklistFriendCodes.size());
                }
                static std::string newBFriendCode = "";
                bool isInBlacklistAlready = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), newBFriendCode) != State.BlacklistFriendCodes.end();
                InputString("Nuevo Código de Amigo", &newBFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInBlacklistAlready)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡Este usuario ya está en la lista negra!");
                if (newBFriendCode != "" && !isInBlacklistAlready) ImGui::SameLine();
                if (newBFriendCode != "" && !isInBlacklistAlready && AnimatedButton("Añadir")) {
                    State.BlacklistFriendCodes.push_back(newBFriendCode);
                    State.Save();
                    newBFriendCode = "";
                }

                if (!State.BlacklistFriendCodes.empty()) {
                    static int selectedBCodeIndex = 0;
                    selectedBCodeIndex = std::clamp(selectedBCodeIndex, 0, (int)State.BlacklistFriendCodes.size() - 1);
                    std::vector<const char*> bCodeVector(State.BlacklistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.BlacklistFriendCodes.size(); i++) {
                        bCodeVector[i] = State.BlacklistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("Jugador a Eliminar", &selectedBCodeIndex, bCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar"))
                        State.BlacklistFriendCodes.erase(State.BlacklistFriendCodes.begin() + selectedBCodeIndex);
                }
            }
            if (State.SMAC_IgnoreWhitelist) {
                ImGui::Text("Lista Blanca");
                if (State.WhitelistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡No hay usuarios en la lista blanca!");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d Usuarios en Lista Blanca)", State.WhitelistFriendCodes.size());
                }
                static std::string newWFriendCode = "";
                static bool isInWhitelistAlready = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), newWFriendCode) != State.WhitelistFriendCodes.end();
                InputString("Nuevo Código de Amigo\n", &newWFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInWhitelistAlready)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡Este usuario ya está en la lista blanca!");
                if (newWFriendCode != "" && !isInWhitelistAlready) ImGui::SameLine();
                if (newWFriendCode != "" && !isInWhitelistAlready && AnimatedButton("Añadir\n")) {
                    State.WhitelistFriendCodes.push_back(newWFriendCode);
                    State.Save();
                    newWFriendCode = "";
                }

                if (!State.WhitelistFriendCodes.empty()) {
                    static int selectedWCodeIndex = 0;
                    selectedWCodeIndex = std::clamp(selectedWCodeIndex, 0, (int)State.WhitelistFriendCodes.size() - 1);
                    std::vector<const char*> wCodeVector(State.WhitelistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.WhitelistFriendCodes.size(); i++) {
                        wCodeVector[i] = State.WhitelistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("Jugador a Eliminar\n", &selectedWCodeIndex, wCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar\n"))
                        State.WhitelistFriendCodes.erase(State.WhitelistFriendCodes.begin() + selectedWCodeIndex);
                }
            }
            ImGui::Text("Detectar Acciones:");
            if (ToggleButton("Uso de Trampas Conocidas", &State.SMAC_CheckOtherCheats)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Uso de SickoMenu", &State.SMAC_CheckSicko)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Nombres Anormales", &State.SMAC_CheckBadNames)) State.Save();

            if (ToggleButton("Cambio de Color Anormal", &State.SMAC_CheckColor)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Cambio de Cosméticos Anormal", &State.SMAC_CheckCosmetics)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Nota de Chat Anormal", &State.SMAC_CheckChatNote)) State.Save();

            if (ToggleButton("Escáner Anormal", &State.SMAC_CheckScanner)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Animación Anormal", &State.SMAC_CheckAnimation)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Asignación de Tareas", &State.SMAC_CheckTasks)) State.Save();

            if (ToggleButton("Asesinatos Anormales", &State.SMAC_CheckMurder)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Metamorfosis Anormal", &State.SMAC_CheckShapeshift)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Desaparición Anormal", &State.SMAC_CheckVanish)) State.Save();


            if (ToggleButton("Reuniones/Reportes Anormales", &State.SMAC_CheckReport)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Ventilación Anormal", &State.SMAC_CheckVent)) State.Save();
            ImGui::SameLine();
           
            if (ToggleButton("Chat Anormal", &State.SMAC_CheckChat)) State.Save();

            if (ToggleButton("Completar Tareas Anormal", &State.SMAC_CheckTaskCompletion)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Sabotajes Anormales", &State.SMAC_CheckSabotage)) State.Save();
            
            if (ToggleButton("Código de Amigo Anormal", &State.SMAC_CheckFriendcode)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Suplantación de Plataforma", &State.SMAC_CheckPlatformSpoof)) State.Save();

            if (ToggleButton("Niveles de Jugador Anormales (0 para ignorar)", &State.SMAC_CheckLevel)) State.Save();

            if (State.SMAC_CheckLevel) {
                ImGui::InputInt("Nivel >=", &State.SMAC_HighLevel);
                ImGui::InputInt("Nivel <=", &State.SMAC_LowLevel);
            }
            if (ToggleButton("Palabras Bloqueadas", &State.SMAC_CheckBadWords)) State.Save();
            if (State.SMAC_CheckBadWords) {
                static const std::vector<const char*> SMAC_DETECTION_MODES = { "Detección Estándar", "Detección Estricta" };
                if (State.SMAC_BadWords.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡No hay palabras bloqueadas añadidas!");
                static std::string newWord = "";
                InputString("Nueva Palabra", &newWord, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                if (AnimatedButton("Añadir Palabra")) {
                    std::string newWordLower = strToLower(newWord);
                    bool alreadyExists = std::any_of(State.SMAC_BadWords.begin(), State.SMAC_BadWords.end(),
                        [&](auto& w) { return strToLower(w.first) == newWordLower; });
                    if (!newWord.empty() && !alreadyExists) {
                        State.SMAC_BadWords.push_back({ newWord, false });
                        State.Save();
                        newWord = "";
                    }
                }
                if (!State.SMAC_BadWords.empty()) {
                    static int selectedWordIndex = 0;
                    selectedWordIndex = std::clamp(selectedWordIndex, 0, (int)State.SMAC_BadWords.size() - 1);
                    std::vector<std::string> wordDisplayStrings;
                    for (auto& w : State.SMAC_BadWords)
                        wordDisplayStrings.push_back(w.first + (w.second ? " [Estricto]" : " [Estándar]"));
                    std::vector<const char*> wordVector(wordDisplayStrings.size(), nullptr);
                    for (size_t i = 0; i < wordDisplayStrings.size(); i++)
                        wordVector[i] = wordDisplayStrings[i].c_str();
                    CustomListBoxInt("Palabra a Eliminar", &selectedWordIndex, wordVector);
                    ImGui::SameLine();
                    int badWordMode = State.SMAC_BadWords[selectedWordIndex].second ? 1 : 0;
                    if (CustomListBoxInt("Modo de Detección##badwords", &badWordMode, SMAC_DETECTION_MODES, 130.0f * State.dpiScale)) {
                        State.SMAC_BadWords[selectedWordIndex].second = (badWordMode == 1);
                        State.Save();
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar"))
                        State.SMAC_BadWords.erase(State.SMAC_BadWords.begin() + selectedWordIndex);
                }
            }

            if (ToggleButton("Palabras de Inicio Bloqueadas", &State.SMAC_CheckStartWords)) State.Save();
            if (State.SMAC_CheckStartWords) {
                static const std::vector<const char*> SMAC_DETECTION_MODES = { "Detección Estándar", "Detección Estricta" };
                ImGui::SetNextItemWidth(70.0f * State.dpiScale);
                if (ImGui::InputInt("Infracciones Antes de Acción", &State.SMAC_StartWordsThreshold)) {
                    State.SMAC_StartWordsThreshold = std::clamp(State.SMAC_StartWordsThreshold, 1, 10);
                }
                if (State.SMAC_StartWords.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡No hay palabras de inicio añadidas!");
                static std::string newStartWord = "";
                InputString("Nueva Palabra\n", &newStartWord, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                if (AnimatedButton("Añadir Palabra##StartWord")) {
                    std::string newStartWordLower = strToLower(newStartWord);
                    bool alreadyExists = std::any_of(State.SMAC_StartWords.begin(), State.SMAC_StartWords.end(),
                        [&](auto& w) { return strToLower(w.first) == newStartWordLower; });
                    if (!newStartWord.empty() && !alreadyExists) {
                        State.SMAC_StartWords.push_back({ newStartWord, false });
                        State.Save();
                        newStartWord = "";
                    }
                }
                if (!State.SMAC_StartWords.empty()) {
                    static int selectedStartWordIndex = 0;
                    selectedStartWordIndex = std::clamp(selectedStartWordIndex, 0, (int)State.SMAC_StartWords.size() - 1);
                    std::vector<std::string> startWordDisplayStrings;
                    for (auto& w : State.SMAC_StartWords)
                        startWordDisplayStrings.push_back(w.first + (w.second ? " [Estricto]" : " [Estándar]"));
                    std::vector<const char*> startWordVector(startWordDisplayStrings.size(), nullptr);
                    for (size_t i = 0; i < startWordDisplayStrings.size(); i++)
                        startWordVector[i] = startWordDisplayStrings[i].c_str();
                    CustomListBoxInt("Palabra de Inicio a Eliminar", &selectedStartWordIndex, startWordVector);
                    ImGui::SameLine();
                    int startWordMode = State.SMAC_StartWords[selectedStartWordIndex].second ? 1 : 0;
                    if (CustomListBoxInt("Modo de Detección##startwords", &startWordMode, SMAC_DETECTION_MODES, 130.0f * State.dpiScale)) {
                        State.SMAC_StartWords[selectedStartWordIndex].second = (startWordMode == 1);
                        State.Save();
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar##StartWord")) {
                        State.SMAC_StartWords.erase(State.SMAC_StartWords.begin() + selectedStartWordIndex);
                        State.Save();
                    }
                }
            }
        }

        if (openUtils) {
            /*if (ToggleButton("Ignore Whitelisted Players [Exploits]", &State.Destruct_IgnoreWhitelist)) {
                State.Save();
            }*/
            if (ToggleButton("Ignorar Jugadores en Lista Blanca [Ban/Expulsar]", &State.Ban_IgnoreWhitelist)) {
                State.Save();
            }

            if (IsInLobby() && ToggleButton("Intentar Crashear la Sala", &State.CrashSpamReport)) {
                State.Save();
            }

            if (ToggleButton("Aparecer Jugadores en Conductos Aleatorios", &State.RandomSpawns)) {
                State.Save();
            }

            if (State.CrashSpamReport) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Cuando comience la partida, la sala será destruida"));

            /*if (!IsInGame() && !IsInLobby()) {
                if (ToggleButton("Overflow", &State.Overflow)) {
                    State.Save();
                }

                if (State.Overflow) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Players who joined a lobby before you are disconnected after 30 seconds"));
            }*/

            if (State.AprilFoolsMode) {
                ImGui::TextColored(ImVec4(0.79f, 0.03f, 1.f, 1.f), State.DiddyPartyMode ? "Modo Fiesta Diddy" : (IsChatCensored() || IsStreamerMode() ? "Modo F***son" : "Modo Fuckson"));
                if (ToggleButton("Hacer Mog a Todos [Sigma]", &State.BrainrotEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }
                if (/*State.DiddyPartyMode && */ToggleButton("Rizzear a Todos [Skibidi]", &State.RizzUpEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                    State.Save();
                }
            }
            if (IsHost()) {
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (((IsInGame() && Object_1_IsNotNull((Object_1*)*Game::pShipStatus)) || (IsInLobby() && Object_1_IsNotNull((Object_1*)*Game::pLobbyBehaviour)))
                    && AnimatedButton(IsInLobby() ? "Eliminar Sala" : "Eliminar Mapa")) {
                    State.taskRpcQueue.push(new DestroyMap());
                }
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                if (ToggleButton("Banear a Todos", &State.BanEveryone)) {
                    State.Save();
                }
                if (ToggleButton("Expulsar a Todos", &State.KickEveryone)) {
                    State.Save();
                }
                SteppedSliderFloat("Retardo de Expulsión/Baneo", &State.AutoPunishDelay, 0.f, 10.f, 0.1f, "%.1f", ImGuiSliderFlags_NoInput);
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                const char* buttonLabel = IsInGame() ? "Expulsar Jugadores AFK" : "Expulsar Jugadores AFK [SOLO EN PARTIDA]";
                if (ToggleButton(buttonLabel, &State.KickAFK)) {
                    State.Save();
                }
                if (State.KickAFK) ImGui::SameLine();
                if (State.KickAFK && ToggleButton("Activar Notificaciones AFK", &State.NotificationsAFK)) {
                    State.Save();
                }
                if (State.KickAFK && ToggleButton("AFK - Segunda Oportunidad", &State.SecondChance)) {
                    State.Save();
                }
                std::string header = "Anti AFK ~ Opciones Avanzadas";
                if (!IsInGame()) {
                    header += " [PARTIDA]";
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (State.KickAFK && ImGui::CollapsingHeader(header.c_str()))
                {
                    SteppedSliderFloat("Tiempo Antes de Expulsar", &State.TimerAFK, 40.f, 350.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput);
                    if (State.SecondChance) {
                        SteppedSliderFloat("Tiempo Extra", &State.AddExtraTime, 15.f, 120.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput);
                        SteppedSliderFloat("Tiempo Mínimo Antes de Añadir", &State.ExtraTimeThreshold, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput);
                    }
                    if (State.NotificationsAFK) {
                        SteppedSliderFloat("Tiempo de Notificación Aviso-AFK", &State.NotificationTimeWarn, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput);
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("Solo Jugadores en Lista Blanca", &State.KickByWhitelist)) {
                    State.Save();
                }
                if (State.KickByWhitelist) ImGui::SameLine();
                if (State.KickByWhitelist && ToggleButton("Activar Notificaciones de LB", &State.WhitelistNotifications)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                if (ToggleButton("Banear Jugadores que se Reconectan Repetidamente", &State.BanLeavers)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (ImGui::CollapsingHeader("BRRP ~ Opciones Avanzadas"))
                {
                    SteppedSliderFloat("Reconexiones Máximas", &State.LeaveCount, 1.f, 15.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput);
                    ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                    if (ToggleButton("Lista Negra para Quien se Reconecte Repetidamente", &State.BL_AutoLeavers)) {
                        State.Save();
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("Avisar/Expulsar por Name-Checker", &State.KickByLockedName)) {
                    State.Save();
                }
                if (State.KickByLockedName) ImGui::SameLine();
                if (State.KickByLockedName && ToggleButton("Mostrar Notificaciones de Datos de Jugador", &State.ShowPDataByNC)) {
                    State.Save();
                }
                if (State.KickByLockedName) {
                    ImGui::Text("Nombres Bloqueados");
                    if (State.LockedNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "¡No hay usuarios en Name-Checker!");
                    static std::string newName = "";
                    InputString("Nuevo Apodo", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    if (newName != "") ImGui::SameLine();
                    if (newName != "" && AnimatedButton("Añadir")) {
                        newName = strToLower(newName);
                        State.LockedNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }

                    if (!State.LockedNames.empty()) {
                        static int selectedName = 0;
                        selectedName = std::clamp(selectedName, 0, (int)State.LockedNames.size() - 1);
                        std::vector<const char*> bNameVector(State.LockedNames.size(), nullptr);
                        for (size_t i = 0; i < State.LockedNames.size(); i++) {
                            bNameVector[i] = State.LockedNames[i].c_str();
                        }
                        CustomListBoxInt("Apodo a Eliminar", &selectedName, bNameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Eliminar"))
                            State.LockedNames.erase(State.LockedNames.begin() + selectedName);
                    }
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                ImGui::BeginGroup();
                if (ToggleButton("Expulsar Jugadores Advertidos", &State.KickWarned)) {
                    State.Save();
                }
                if (ToggleButton("Banear Jugadores Advertidos", &State.BanWarned)) {
                    State.Save();
                }
                if (ToggleButton("Notificar a Jugador Advertido", &State.NotifyWarned)) {
                    State.Save();
                }

                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

                ImGui::PushItemWidth(80);
                ImGui::InputInt("Avisos Máx", &State.MaxWarns);
                if (State.MaxWarns < 1)
                    State.MaxWarns = 1;
                ImGui::PopItemWidth();
                ImGui::EndGroup();
            }
            if (IsHost()) ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushItemWidth(150);
            if (!IsHost()) ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ImGui::Combo("Modo de Vista de Avisos", &selectedWarnView, warnViewModes, WarnView_COUNT);
            ImGui::PopItemWidth();


            if (selectedWarnView == WarnView_List) {
                if (!State.WarnedFriendCodes.empty()) {
                    ImGui::Text("Jugadores Advertidos");

                    std::string localFC = "";
                    if (Game::pLocalPlayer && *Game::pLocalPlayer) {
                        localFC = convert_from_string((*Game::pLocalPlayer)->fields.FriendCode);
                    }

                    std::vector<std::string> warnedList;
                    std::vector<std::string> fcKeys;

                    for (const auto& [fc, count] : State.WarnedFriendCodes) {
                        if (count <= 0 || fc == localFC)
                            continue;

                        warnedList.push_back(std::format("{} ({} aviso{})", fc, count, count == 1 ? "" : "s"));
                        fcKeys.push_back(fc);
                    }

                    if (!warnedList.empty()) {
                        static int selectedWarned = 0;
                        selectedWarned = std::clamp(selectedWarned, 0, (int)warnedList.size() - 1);

                        std::vector<const char*> warnedCStrs;
                        for (const auto& entry : warnedList) warnedCStrs.push_back(entry.c_str());

                        ImGui::PushItemWidth(200);
                        CustomListBoxInt("Códigos de Amigo Advertidos", &selectedWarned, warnedCStrs);
                        ImGui::PopItemWidth();

                        ImGui::SameLine();
                        if (ImGui::Button("Eliminar")) {
                            if (selectedWarned >= 0 && selectedWarned < (int)fcKeys.size()) {
                                std::string fc = fcKeys[selectedWarned];
                                State.WarnedFriendCodes.erase(fc);
                                State.WarnReasons.erase(fc);
                                selectedWarned = 0;
                                State.Save();
                            }
                        }

                        std::string selectedFc = fcKeys[selectedWarned];
                        auto& warnReasons = State.WarnReasons[selectedFc];

                        if (!warnReasons.empty()) {
                            ImGui::Text("Razones de Advertencia:");

                            static int selectedReason = 0;
                            selectedReason = std::clamp(selectedReason, 0, (int)warnReasons.size() - 1);

                            std::vector<std::string> numberedReasons;
                            numberedReasons.reserve(warnReasons.size());
                            for (size_t i = 0; i < warnReasons.size(); ++i) {
                                numberedReasons.push_back(std::format("[{}] {}", i + 1, warnReasons[i]));
                            }

                            std::vector<const char*> reasonCStrs;
                            for (const auto& str : numberedReasons) reasonCStrs.push_back(str.c_str());

                            ImGui::PushItemWidth(200);
                            ImGui::ListBox("##WarnReasonList", &selectedReason, reasonCStrs.data(), (int)reasonCStrs.size());
                            ImGui::PopItemWidth();

                            ImGui::SameLine();
                            if (ImGui::Button("Eliminar##warnreason")) {
                                if (selectedReason >= 0 && selectedReason < (int)warnReasons.size()) {
                                    warnReasons.erase(warnReasons.begin() + selectedReason);
                                    selectedReason = 0;

                                    if (--State.WarnedFriendCodes[selectedFc] <= 0) {
                                        State.WarnedFriendCodes.erase(selectedFc);
                                        State.WarnReasons.erase(selectedFc);
                                        selectedWarned = 0;
                                    }

                                    State.Save();
                                }
                            }
                        }
                    }
                    else {
                        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "No hay jugadores advertidos.");
                    }
                }
            }
            else if (selectedWarnView == WarnView_Manual) {
                static std::string friendCodeToWarn;
                static std::string warnReason;

                ImGui::PushItemWidth(200);
                InputString("Código de Amigo##warn", &friendCodeToWarn);
                InputString("Razón", &warnReason);
                ImGui::PopItemWidth();

                if (ImGui::Button("Enviar Advertencia") && !friendCodeToWarn.empty() && !warnReason.empty()) {
                    State.WarnedFriendCodes[friendCodeToWarn]++;
                    State.WarnReasons[friendCodeToWarn].push_back(warnReason);
                    State.Save();

                    friendCodeToWarn.clear();
                    warnReason.clear();
                }
            }

            ImGui::EndGroup();

            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

            if (ToggleButton("Activar Sistema de Baneo Temporal", &State.TempBanEnabled)) {
                State.Save();
            }
            if (State.TempBanEnabled && ImGui::CollapsingHeader("Sistema de Baneo Temporal")) {
                static std::string friendCodeToTempBan;
                static int banDays = 0, banHours = 0, banMinutes = 0, banSeconds = 0;

                ImGui::BeginGroup();
                ImGui::PushItemWidth(150);
                InputString("Código de Amigo", &friendCodeToTempBan);

                ImGui::InputInt("Días", &banDays);     banDays = std::max<int>(0, banDays);
                ImGui::InputInt("Horas", &banHours);   banHours = std::clamp(banHours, 0, 23);
                ImGui::InputInt("Minutos", &banMinutes); banMinutes = std::clamp(banMinutes, 0, 59);
                ImGui::InputInt("Segundos", &banSeconds); banSeconds = std::clamp(banSeconds, 0, 59);
                ImGui::PopItemWidth();

                if (!friendCodeToTempBan.empty() && ImGui::Button("Aplicar Baneo Temporal")) {
                    std::string selfFC;
                    if (Game::pLocalPlayer && *Game::pLocalPlayer) {
                        selfFC = convert_from_string((*Game::pLocalPlayer)->fields.FriendCode);
                    }

                    if (!selfFC.empty() && friendCodeToTempBan == selfFC) { }
                    else {
                        int64_t totalSeconds = 0;
                        totalSeconds += static_cast<int64_t>(banDays) * 86400;
                        totalSeconds += static_cast<int64_t>(banHours) * 3600;
                        totalSeconds += static_cast<int64_t>(banMinutes) * 60;
                        totalSeconds += static_cast<int64_t>(banSeconds);

                        if (totalSeconds > State.MAX_BAN_SECONDS) {
                            totalSeconds = State.MAX_BAN_SECONDS;
                        }

                        if (totalSeconds > 0) {
                            auto now = std::chrono::system_clock::now();
                            auto banEnd = now + std::chrono::seconds(totalSeconds);

                            State.TempBannedFCs[friendCodeToTempBan] = banEnd;
                            State.Save();

                            if (IsInGame() || IsInLobby()) {
                                for (auto p : GetAllPlayerControl()) {
                                    if (!p) continue;
                                    if (convert_from_string(p->fields.FriendCode) == friendCodeToTempBan) {
                                        // Main & first ban (new temp-banned user):
                                        if (IsInGame())
                                             State.rpcQueue.push(new PunishPlayer(p, false));
                                        if (IsInLobby())
                                             State.lobbyRpcQueue.push(new PunishPlayer(p, false));
                                    }
                                }
                            }
                        }
                    }
                }
                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::Text("Jugadores Baneados Temporalmente:");

                auto now = std::chrono::system_clock::now();
                if (State.TempBannedFCs.empty()) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Ningún jugador está baneado temporalmente.");
                }
                else {
                    static int selectedTempBanIndex = 0;
                    std::vector<std::string> displayList, friendCodeList;

                    for (const auto& [fc, until] : State.TempBannedFCs) {
                        auto timeLeft = std::chrono::duration_cast<std::chrono::seconds>(until - now).count();
                        if (timeLeft < 0) timeLeft = 0;

                        int d = (int)(timeLeft / 86400);
                        int h = (int)((timeLeft % 86400) / 3600);
                        int m = (int)((timeLeft % 3600) / 60);
                        int s = (int)(timeLeft % 60);

                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "%s | %02dd:%02dh:%02dm:%02ds", fc.c_str(), d, h, m, s);

                        displayList.push_back(buffer);
                        friendCodeList.push_back(fc);
                    }

                    std::vector<const char*> displayCStrs;
                    for (auto& s : displayList) displayCStrs.push_back(s.c_str());

                    selectedTempBanIndex = std::clamp(selectedTempBanIndex, 0, (int)displayCStrs.size() - 1);
                    CustomListBoxInt("Seleccionar Baneo Temporal", &selectedTempBanIndex, displayCStrs);

                    if (ImGui::Button("Desbanear")) {
                        if (selectedTempBanIndex >= 0 && selectedTempBanIndex < (int)friendCodeList.size()) {
                            std::string targetFC = friendCodeList[selectedTempBanIndex];
                            State.TempBannedFCs.erase(targetFC);
                            State.Save();
                        }
                    }
                }

                ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Nota: Funciones de Baneo Temporal\n¡Solo funcionan siendo Anfitrión!");
                ImGui::EndGroup();
            }
        }

        if (openHistory) {
            ImGui::Dummy(ImVec2(0, 3)* State.dpiScale);
            if (ImGui::CollapsingHeader("Historial de Jugadores", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Últimos 100 jugadores:");

            static std::string historySearchBuf = "";
            ImGui::SetNextItemWidth(200);
            InputString("##HistorySearch", &historySearchBuf);
            ImGui::SameLine();
            ImGui::TextDisabled("Buscar");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Filtrar por nombre, código de amigo o PUID");

            static int selectedIndex = -1;

            static std::string lastSearchQuery = "";
            if (historySearchBuf != lastSearchQuery) {
                lastSearchQuery = historySearchBuf;
                selectedIndex = -1;
            }

            std::vector<std::string> decoratedStorage;
            decoratedStorage.reserve(State.PlayerHistory.size());
            std::vector<const char*> names;
            std::vector<int> filteredIndices;

            names.reserve(State.PlayerHistory.size());
            filteredIndices.reserve(State.PlayerHistory.size());

            std::string searchQuery = historySearchBuf;
            std::transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);

            for (int i = 0; i < (int)State.PlayerHistory.size(); ++i)
            {
                auto& p = State.PlayerHistory[i];
                auto itf = State.platformFilters.find(p.Platform);
                bool visible = (itf != State.platformFilters.end()) ? itf->second : true;
                if (!visible) continue;

                if (!searchQuery.empty()) {
                    std::string lnick = p.Nick;
                    std::transform(lnick.begin(), lnick.end(), lnick.begin(), ::tolower);
                    std::string lfc = p.FriendCode;
                    std::transform(lfc.begin(), lfc.end(), lfc.begin(), ::tolower);
                    std::string lpuid = p.Puid;
                    std::transform(lpuid.begin(), lpuid.end(), lpuid.begin(), ::tolower);

                    if (lnick.find(searchQuery) == std::string::npos &&
                        lfc.find(searchQuery) == std::string::npos &&
                        lpuid.find(searchQuery) == std::string::npos)
                        continue;
                }
                std::string decorated = p.Nick;

                if (p.NameCheck) decorated = "[!] " + decorated;
                bool inWL = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), p.FriendCode) != State.WhitelistFriendCodes.end();
                bool inBL = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), p.FriendCode) != State.BlacklistFriendCodes.end();
                if (inWL) decorated = "[+] " + decorated;
                if (inBL) decorated = "[-] " + decorated;

                decoratedStorage.push_back(std::move(decorated));
                names.push_back(decoratedStorage.back().c_str());
                filteredIndices.push_back(i);
            }

            if (names.empty())
            {
                selectedIndex = -1;
            }
            else
            {
                if (selectedIndex >= (int)names.size()) selectedIndex = (int)names.size() - 1;

                ImGui::PushItemWidth(200);
                if (ImGui::ListBox("##PlayerList", &selectedIndex, names.data(), (int)names.size(), 10))
                {
                    if (selectedIndex < 0 || selectedIndex >= (int)filteredIndices.size()) selectedIndex = -1;
                }
                ImGui::PopItemWidth();

                if (selectedIndex >= 0)
                {
                    int realIndex = filteredIndices[selectedIndex];
                    auto& p = State.PlayerHistory[realIndex];

                    ImGui::SameLine();
                    ImGui::BeginGroup();

                    ImGui::Text("Usa Cliente Modificado: %s", p.IsModded ? "Sí" : "No");
                    if (p.IsModded && !p.ModClient.empty()) ImGui::Text("Nombre del Cliente: %s", p.ModClient.c_str());
                    ImGui::NewLine();
                    ImGui::Text("Código de Amigo: %s", p.FriendCode.c_str());
                    ImGui::Text("PUID: %s", p.Puid.c_str());
                    ImGui::Text("Nivel: %d", p.Level);
                    ImGui::Text("Plataforma: %s", p.Platform.c_str());
                    ImGui::Text("Name-Checker: %s", p.NameCheck ? "Sí" : "Ninguno");
                    ImGui::NewLine();

                    if (AnimatedButton("Copiar Información"))
                    {
                        std::string infoText = "Información de " + p.Nick + ":\n" +
                            "Plataforma: " + p.Platform + "\n" +
                            "Nivel: " + std::format("{}", p.Level) + "\n" +
                            "PUID: " + p.Puid + "\n" +
                            "Código de Amigo: " + p.FriendCode + "\n"
                            "Usa Cliente Modificado: " + (p.IsModded ? "Sí" : "No") + "\n" +
                            (p.IsModded && !p.ModClient.empty() ? ("Nombre del Cliente: " + p.ModClient + "\n") : "") +
                            "En Name-Checker: " + (p.NameCheck ? "Sí" : "No");
                        ClipboardHelper_PutClipboardString(convert_to_string(infoText), NULL);
                    }

                    if (AnimatedButton("Borrar Jugador"))
                    {
                        State.RemovedPlayers.insert(p.FriendCode);
                        State.PlayerHistory.erase(State.PlayerHistory.begin() + realIndex);
                        State.Save();
                        selectedIndex = -1;
                    }

                    ImGui::EndGroup();
                    ImGui::Spacing();

                    bool inWL = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), p.FriendCode) != State.WhitelistFriendCodes.end();
                    std::string wLabel = inWL ? "Quitar de Lista Blanca" : "Añadir a Lista Blanca";

                    if (AnimatedButton(wLabel.c_str()))
                    {
                        if (inWL)
                            RemoveFromWhitelist(p.FriendCode);
                        else
                        {
                            AddToWhitelist(p.FriendCode);
                            RemoveFromBlacklist(p.FriendCode);
                        }
                        State.Save();
                        p;
                    }

                    ImGui::SameLine();

                    bool inBL = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), p.FriendCode) != State.BlacklistFriendCodes.end();
                    std::string bLabel = inBL ? "Quitar de Lista Negra" : "Añadir a Lista Negra";

                    if (AnimatedButton(bLabel.c_str()))
                    {
                        if (inBL)
                            RemoveFromBlacklist(p.FriendCode);
                        else
                        {
                            AddToBlacklist(p.FriendCode);
                            RemoveFromWhitelist(p.FriendCode);
                        }
                        State.Save();
                    }

                    ImGui::SameLine();

                    std::string lowName = p.Nick;
                    std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
                    std::string ncLabel = p.NameCheck ? "Quitar de Name-Checker" : "Añadir a Name-Checker";

                    if (AnimatedButton(ncLabel.c_str()))
                    {
                        if (p.NameCheck)
                        {
                            State.LockedNames.erase(std::remove(State.LockedNames.begin(), State.LockedNames.end(), lowName), State.LockedNames.end());
                            p.NameCheck = false;
                        }
                        else
                        {
                            State.LockedNames.push_back(lowName);
                            p.NameCheck = true;
                        }
                        for (auto& rp : State.PlayerHistory) {
                            std::string lc = rp.Nick;
                            std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
                            rp.NameCheck = (std::find(State.LockedNames.begin(), State.LockedNames.end(), lc) != State.LockedNames.end());
                        }
                        State.Save();
                    }
                }
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

            if (ImGui::Button("Borrar Historial"))
            {
                for (auto& pp : State.PlayerHistory) State.RemovedPlayers.insert(pp.FriendCode);
                State.PlayerHistory.clear();
                selectedIndex = -1;
                State.Save();
            }
            ImGui::SameLine(0, 20);
            if (ImGui::Button("Actualizar Historial de Jugadores"))
            {
                bool changed = false;
                for (auto pctrl : GetAllPlayerControl())
                {
                    if (!pctrl || pctrl == *Game::pLocalPlayer) continue;
                    auto data = GetPlayerData(pctrl);
                    if (!data || data->fields.Disconnected) continue;

                    std::string fc = convert_from_string(data->fields.FriendCode);
                    std::string name = strToLower(RemoveHtmlTags(convert_from_string(GetPlayerOutfit(data)->fields.PlayerName)));
                    std::string puid = convert_from_string(data->fields.Puid);
                    int level = data->fields.PlayerLevel + 1;

                    if (fc.empty() || name.empty() || level <= 0) continue;
                    if (State.RemovedPlayers.count(fc)) State.RemovedPlayers.erase(fc);

                    bool exists = false;
                    for (auto& rp : State.PlayerHistory) if (rp.FriendCode == fc) { exists = true; break; }
                    if (exists) continue;

                    std::string platform = "Unknown";
                    auto client = app::InnerNetClient_GetClientFromCharacter((InnerNetClient*)(*Game::pAmongUsClient), pctrl, NULL);
                    if (client != nullptr && client->fields.PlatformData != nullptr && pctrl->fields._.OwnerId == client->fields.Id) {
                        switch (client->fields.PlatformData->fields.Platform) {
                        case Platforms__Enum::StandaloneEpicPC:
                            platform = "Epic Games (PC)";
                            break;
                        case Platforms__Enum::StandaloneSteamPC:
                            platform = "Steam (PC)";
                            break;
                        case Platforms__Enum::StandaloneMac:
                            platform = "Mac";
                            break;
                        case Platforms__Enum::StandaloneWin10:
                            platform = "Microsoft Store (PC)";
                            break;
                        case Platforms__Enum::StandaloneItch:
                            platform = "itch.io (PC)";
                            break;
                        case Platforms__Enum::IPhone:
                            platform = "iOS/iPadOS (Mobile)";
                            break;
                        case Platforms__Enum::Android:
                            platform = "Android (Mobile)";
                            break;
                        case Platforms__Enum::Switch:
                            platform = "Nintendo Switch (Console)";
                            break;
                        case Platforms__Enum::Xbox:
                            platform = "Xbox (Console)";
                            break;
                        case Platforms__Enum::Playstation:
                            platform = "Playstation (Console)";
                            break;
                        default:
                            platform = "Unknown";
                            break;
                        }
                    }

                    std::string lcname = name;
                    std::transform(lcname.begin(), lcname.end(), lcname.begin(), ::tolower);
                    bool nameCheck = (std::find(State.LockedNames.begin(), State.LockedNames.end(), lcname) != State.LockedNames.end());

                    bool isCheater = false;
                    std::string cheatName = "";
                    int pid = data->fields.PlayerId;
                    auto modIt = State.modUsers.find(pid);
                    if (modIt != State.modUsers.end()) {
                        std::string modVersionDisplay = modIt->second[1].empty() ? "" : " " + modIt->second[1];
                        cheatName = RemoveHtmlTags(modIt->second[0] + modVersionDisplay);
                        isCheater = true;
                    }

                    if (State.PlayerHistory.size() >= 100)
                        State.PlayerHistory.pop_front();

                    State.PlayerHistory.push_back({ name, fc, puid, level, platform, nameCheck, isCheater, cheatName });
                    changed = true;
                }
                if (changed) State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

            if (ImGui::CollapsingHeader("Filtros de Plataforma"))
            {
                ImGui::Columns(2, NULL, false);

                for (size_t i = 0; i < PLATFORM_FILTERS.size(); i++)
                {
                    ToggleButton(PLATFORM_FILTERS[i].c_str(), &State.platformFilters[PLATFORM_FILTERS[i]]);

                    if (i == (PLATFORM_FILTERS.size() + 1) / 2 - 1)
                        ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }

            ImGui::Dummy(ImVec2(5, 5)* State.dpiScale);
            }

            if (ImGui::CollapsingHeader("Historial de Salas", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (State.LobbyHistory.empty()) {
                    ImGui::TextDisabled("Aún no has visitado ninguna sala.");
                }
                else {
                    SliderIntV2("Salas a Mostrar", &State.LobbyHistoryLimit, 1, 50, "%d", ImGuiSliderFlags_NoInput);
                    int displayCount = (int)State.LobbyHistory.size() < State.LobbyHistoryLimit ? (int)State.LobbyHistory.size() : State.LobbyHistoryLimit;
                    ImGui::Text("Últimas %d/%d salas:", displayCount, State.LobbyHistoryLimit);
                    ImGui::Columns(3, "lobbyHistoryCols", false);
                    ImGui::SetColumnWidth(0, 80 * State.dpiScale);
                    ImGui::SetColumnWidth(1, 120 * State.dpiScale);
                    ImGui::SetColumnWidth(2, 160 * State.dpiScale);

                    ImGui::TextDisabled("Código"); ImGui::NextColumn();
                    ImGui::TextDisabled("Anfitrión"); ImGui::NextColumn();
                    ImGui::NextColumn();
                    ImGui::Separator();

                    int lobbyCount = 0;
                    for (auto& lobby : State.LobbyHistory) {
                        if (lobbyCount >= State.LobbyHistoryLimit) break;
                        lobbyCount++;
                        ImGui::Text("%s", lobby.Code.c_str());
                        ImGui::NextColumn();
                        ImGui::Text("%s", lobby.HostName.empty() ? "Desconocido" : lobby.HostName.c_str());
                        ImGui::NextColumn();
                        if (AnimatedButton(("Copiar##" + lobby.Code).c_str()))
                            ImGui::SetClipboardText(lobby.Code.c_str());
                        ImGui::SameLine();
                        if (AnimatedButton(("Unirse##" + lobby.Code).c_str())) {
                            State.JoinLobbyCode = lobby.Code;
                            State.JoinLobby = true;
                        }
                        ImGui::SameLine();
                        if (AnimatedButton(("Borrar##" + lobby.Code).c_str())) {
                            State.LobbyHistory.erase(std::remove_if(State.LobbyHistory.begin(), State.LobbyHistory.end(),
                                [&lobby](const auto& l) { return l.Code == lobby.Code; }), State.LobbyHistory.end());
                            break;
                        }
                        ImGui::NextColumn();
                    }
                    ImGui::Columns(1);

                    ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                    if (AnimatedButton("Borrar Historial##lobby"))
                        State.LobbyHistory.clear();
                }
            }
        }

        if (openOptions) {
            if ((IsInGame() || IsInLobby()) && GameOptions().HasOptions()) {
                GameOptions options;
                /*std::string hostText = std::format("Host: {}", RemoveHtmlTags(GetHostUsername()));
                ImGui::Text(const_cast<char*>(hostText.c_str()));*/

                if (options.GetGameMode() == GameModes__Enum::Normal)
                {
                    auto allPlayers = GetAllPlayerControl();
                    RoleRates roleRates = RoleRates(options, (int)allPlayers.size());
                    // this should be all the major ones. if people want more they're simple enough to add.
                    ImGui::Text("Tareas Visuales: %s", (options.GetBool(app::BoolOptionNames__Enum::VisualTasks) ? "Activado" : "Desactivado"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::TaskBarMode)) {
                    case 0:
                        ImGui::Text("Actualizaciones de Barra de Tareas: Siempre");
                        break;
                    case 1:
                        ImGui::Text("Actualizaciones de Barra de Tareas: En Reuniones");
                        break;
                    case 2:
                        ImGui::Text("Actualizaciones de Barra de Tareas: Nunca");
                        break;
                    default:
                        ImGui::Text("Actualizaciones de Barra de Tareas: Otro");
                        break;
                    }
                    ImGui::Text("Confirmar Expulsiones: %s", (options.GetBool(app::BoolOptionNames__Enum::ConfirmImpostor) ? "Activado" : "Desactivado"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::KillDistance)) {
                    case 0:
                        ImGui::Text("Distancia de Asesinato: Corta");
                        break;
                    case 1:
                        ImGui::Text("Distancia de Asesinato: Media");
                        break;
                    case 2:
                        ImGui::Text("Distancia de Asesinato: Larga");
                        break;
                    default:
                        ImGui::Text("Distancia de Asesinato: Otra");
                        break;
                    }

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Ingenieros Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Engineer));
                    ImGui::Text("Probabilidad de Ingeniero: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Engineer));
                    ImGui::Text("Recarga de Conducto Ingeniero: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerCooldown, 1.0F));
                    ImGui::Text("Duración en Conducto Ingeniero: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerInVentMaxTime, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Científicos Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Scientist));
                    ImGui::Text("Probabilidad de Científico: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Scientist));
                    ImGui::Text("Recarga de Vitales Científico: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistCooldown, 1.0F));
                    ImGui::Text("Duración de Batería Científico: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistBatteryCharge, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Bocineros Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Noisemaker));
                    ImGui::Text("Probabilidad de Bocinero: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Noisemaker));
                    ImGui::Text("Duración de Alerta Bocinero: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::NoisemakerAlertDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Rastreadores Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Tracker));
                    ImGui::Text("Probabilidad de Rastreador: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Tracker));
                    ImGui::Text("Recarga de Rastreo: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDuration, 1.0F));
                    ImGui::Text("Duración de Rastreo: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerCooldown, 1.0F));
                    ImGui::Text("Retardo de Rastreo: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDelay, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Detectives Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Detective));
                    ImGui::Text("Probabilidad de Detective: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Detective));
                    ImGui::Text("Límite de Sospechosos Detective: %.2f", options.GetFloat(app::FloatOptionNames__Enum::DetectiveSuspectLimit, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("Ángeles Guardianes Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("Probabilidad de Ángel Guardián: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("Recarga de Protección de Ángel: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::GuardianAngelCooldown, 1.0F));
                    ImGui::Text("Duración de Protección de Ángel: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ProtectionDurationSeconds, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("Influencers Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::SpiritGuide));
                    ImGui::Text("Probabilidad de Influencer: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::SpiritGuide));
                    ImGui::Text("Recarga de Mensaje Influencer: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::SpiritGuideCooldownSeconds, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("Metamorfos Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("Probabilidad de Metamorfo: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("Recarga de Transformación Metamorfo: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterCooldown, 1.0F));
                    ImGui::Text("Duración de Transformación Metamorfo: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Fantasmas Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Phantom));
                    ImGui::Text("Probabilidad de Fantasma: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Phantom));
                    ImGui::Text("Recarga de Desvanecimiento Fantasma: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomCooldown, 1.0F));
                    ImGui::Text("Duración de Desvanecimiento Fantasma: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Víboras Máx: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Viper));
                    ImGui::Text("Probabilidad de Víbora: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Viper));
                    ImGui::Text("Tiempo de Disolución Víbora: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ViperDissolveTime, 1.0F));
                }
                else if (options.GetGameMode() == GameModes__Enum::HideNSeek) {

                    int ImpostorId = options.GetInt(app::Int32OptionNames__Enum::ImpostorPlayerID);
                    if (ImpostorId < 0) {
                        ImGui::Text("Impostor: Por Turnos");
                    }
                    else {
                        std::string ImpostorName = std::format("Impostor Seleccionado: {}", convert_from_string(NetworkedPlayerInfo_get_PlayerName(GetPlayerDataById(ImpostorId), nullptr)));
                        ImGui::Text(const_cast<char*>(ImpostorName.c_str()));
                    }
                    ImGui::Text("Modo Linterna: %s", (options.GetBool(app::BoolOptionNames__Enum::UseFlashlight) ? "Activado" : "Desactivado"));
                    ImGui::Text("Mostrar Nombres: %s", (options.GetBool(app::BoolOptionNames__Enum::ShowCrewmateNames) ? "Activado" : "Desactivado"));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Usos de Conducto Máx: %d", options.GetInt(app::Int32OptionNames__Enum::CrewmateVentUses));
                    ImGui::Text("Duración en Conducto: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::CrewmateTimeInVent, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("Tiempo de Escondite: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EscapeTime, 1.0F));
                    ImGui::Text("Tiempo Final de Escondite: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::FinalEscapeTime, 1.0F));
                    ImGui::Text("Velocidad Final del Buscador: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::SeekerFinalSpeed, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("Tamaño de Linterna de Quien se Esconde: % .2fx", options.GetFloat(app::FloatOptionNames__Enum::CrewmateFlashlightSize, 1.0F));
                    ImGui::Text("Tamaño de Linterna del Buscador: % .2fx", options.GetFloat(app::FloatOptionNames__Enum::ImpostorFlashlightSize, 1.0F));
                    ImGui::Text("Mapa Final del Buscador: %s", (options.GetBool(app::BoolOptionNames__Enum::SeekerFinalMap) ? "Activado" : "Desactivado"));
                    ImGui::Text("Pings Finales de Escondite: %s", (options.GetBool(app::BoolOptionNames__Enum::SeekerPings) ? "Activado" : "Desactivado"));
                    ImGui::Text("Intervalo de Ping: % .2f s", options.GetFloat(app::FloatOptionNames__Enum::MaxPingTime, 1.0F));
                }
            }
            else CloseOtherGroups(Groups::General);
        }
        ImGui::EndChild();
        ImGui::EndChild();
    }
}
