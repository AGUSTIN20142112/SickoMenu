#include "pch-il2cpp.h"
#include "self_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "toasts.hpp"
#include "logger.h"
#include "_hooks.h"

extern void RevealAnonymousVotes(); // in MeetingHud.cpp

namespace SelfTab {
    enum Groups {
        Visuals,
        Utils,
        Roles,
        Randomizers,
        AntiExploit,
        TextEditor
    };

    static bool openVisuals = true; //default to visual tab group
    static bool openUtils = false;
    static bool openRoles = false;
    static bool openRandomizers = false;
    static bool openAntiExploit = false;
    static bool openTextEditor = false;

    static std::string originalText = "";
    static std::string editedText = "";

    static bool italicName = false;
    static bool underlineName = false;
    static bool strikethroughName = false;
    static bool boldName = false;
    static bool nobrName = false;
    static ImVec4 nameColor1 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static ImVec4 nameColor2 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static bool coloredName = false;
    static bool font = false;
    static int fontType = 0;
    static bool resizeName = false;
    static float nameSize = 0.f;
    static bool indentName = false;
    static float indentLevel = 0.f;
    static bool cspaceName = false;
    static float cspaceLevel = 0.f;
    static bool mspaceName = false;
    static float mspaceLevel = 0.f;
    static bool voffsetName = false;
    static float voffsetLevel = 0.f;
    static bool rotateName = false;
    static float rotateAngle = 0.f;

    void CloseOtherGroups(Groups group) {
        openVisuals = group == Groups::Visuals;
        openUtils = group == Groups::Utils;
        openRoles = group == Groups::Roles;
        openRandomizers = group == Groups::Randomizers;
        openAntiExploit = group == Groups::AntiExploit;
        openTextEditor = group == Groups::TextEditor;
    }

    void OpenSubGroup(const std::string& name) {
        if (name == "Visuals" || name == "Visuales") CloseOtherGroups(Groups::Visuals);
        else if (name == "Utils" || name == "Utilidades") CloseOtherGroups(Groups::Utils);
        else if (name == "Roles") CloseOtherGroups(Groups::Roles);
        else if (name == "Randomizers" || name == "Aleatorios") CloseOtherGroups(Groups::Randomizers);
        else if (name == "Anti-Exploit" || name == "Anti-Exploits") CloseOtherGroups(Groups::AntiExploit);
        else if (name == "Text Editor" || name == "Editor de Texto") CloseOtherGroups(Groups::TextEditor);
    }

    std::string GetTextEditorName(std::string str) {
        str = RemoveHtmlTags(str);

        std::string opener = "", closer = "";

        if (coloredName) {
            str = GetGradientUsername(str, nameColor1, nameColor2);
        }

        if (italicName) {
            opener += "<i>";
            closer += "</i>";
        }

        if (underlineName && !coloredName) {
            opener += "<u>";
            closer += "</u>";
        }

        if (strikethroughName && !coloredName) {
            opener += "<s>";
            closer += "</s>";
        }

        if (boldName) {
            opener += "<b>";
            closer += "</b>";
        }

        if (nobrName) {
            opener += "<nobr>";
            closer += "</nobr>";
        }

        if (font) {
            switch (fontType) {
            case 0: {
                opener += "<font=\"Barlow-Italic SDF\">";
                break;
            }
            case 1: {
                opener += "<font=\"Barlow-Medium SDF\">";
                break;
            }
            case 2: {
                opener += "<font=\"Barlow-Bold SDF\">";
                break;
            }
            case 3: {
                opener += "<font=\"Barlow-SemiBold SDF\">";
                break;
            }
            case 4: {
                opener += "<font=\"Barlow-SemiBold Masked\">";
                break;
            }
            case 5: {
                opener += "<font=\"Barlow-ExtraBold SDF\">";
                break;
            }
            case 6: {
                opener += "<font=\"Barlow-BoldItalic SDF\">";
                break;
            }
            case 7: {
                opener += "<font=\"Barlow-BoldItalic Masked\">";
                break;
            }
            case 8: {
                opener += "<font=\"Barlow-Black SDF\">";
                break;
            }
            case 9: {
                opener += "<font=\"Barlow-Light SDF\">";
                break;
            }
            case 10: {
                opener += "<font=\"Barlow-Regular SDF\">";
                break;
            }
            case 11: {
                opener += "<font=\"Barlow-Regular Masked\">";
                break;
            }
            case 12: {
                opener += "<font=\"Barlow-Regular Outline\">";
                break;
            }
            case 13: {
                opener += "<font=\"Brook SDF\">";
                break;
            }
            case 14: {
                opener += "<font=\"LiberationSans SDF\">";
                break;
            }
            case 15: {
                opener += "<font=\"NotoSansJP-Regular SDF\">";
                break;
            }
            case 16: {
                opener += "<font=\"VCR SDF\">";
                break;
            }
            case 17: {
                opener += "<font=\"CONSOLA SDF\">";
                break;
            }
            case 18: {
                opener += "<font=\"digital-7 SDF\">";
                break;
            }
            case 19: {
                opener += "<font=\"OCRAEXT SDF\">";
                break;
            }
            case 20: {
                opener += "<font=\"DIN_Pro_Bold_700 SDF\">";
                break;
            }
            }
            closer += "</font>";
        }

        /*if (State.Material) {
            switch (State.MaterialType) {
            case 0: {
                opener += "<material=\"Barlow-Italic SDF Outline\">";
                break;
            }
            case 1: {
                opener += "<material=\"Barlow-BoldItalic SDF Outline\">";
                break;
            }
            case 2: {
                opener += "<material=\"Barlow-SemiBold SDF Outline\">";
                break;
            }
                    closer += "</material>";
            }
        }*/

        if (resizeName) {
            opener += std::format("<size={}%>", nameSize * 100);
            closer += "</size>";
        }

        if (indentName) {
            opener += std::format("<line-indent={}>", indentLevel);
            closer += "</line-indent>";
        }

        if (cspaceName) {
            opener += std::format("<cspace={}>", cspaceLevel);
            closer += "</cspace>";
        }

        if (mspaceName) {
            opener += std::format("<mspace={}>", mspaceLevel);
            closer += "</mspace>";
        }

        if (voffsetName) {
            opener += std::format("<voffset={}>", voffsetLevel);
            closer += "</voffset>";
        }

        if (rotateName) {
            opener += std::format("<rotate={}>", rotateAngle);
            closer += "<rotate=0>";
        }

        return opener + str + closer;
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###SelfButtons", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Visuales", openVisuals)) {
            CloseOtherGroups(Groups::Visuals);
        }
        ImGui::SameLine();
        if (TabGroup("Utilidades", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }
        ImGui::SameLine();
        if (TabGroup("Roles", openRoles)) {
            CloseOtherGroups(Groups::Roles);
        }
        ImGui::SameLine();
        if (TabGroup("Aleatorios", openRandomizers)) {
            CloseOtherGroups(Groups::Randomizers);
        }
        ImGui::SameLine();
        if (TabGroup("Anti-Exploit", openAntiExploit)) {
            CloseOtherGroups(Groups::AntiExploit);
        }
        ImGui::SameLine();
        if (TabGroup("Editor de Texto", openTextEditor)) {
            CloseOtherGroups(Groups::TextEditor);
        }

        ImGui::BeginChild("###Self", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (openVisuals) {
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ToggleButton("Vision Maxima", &State.MaxVision)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Atravesar Paredes (Wallhack)", &State.Wallhack)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ocultar HUD (Interfaz)", &State.DisableHud)) {
                if (!IsInGame()) State.DisableHud = false;
            }

            if (ToggleButton("Camara Libre", &State.FreeCam)) {
                State.playerToFollow = {};
                State.Save();
            }

            ImGui::SameLine(130.f * State.dpiScale);
            SteppedSliderFloat("Velocidad", &State.FreeCamSpeed, 0.f, 10.f, 0.1f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

            if (ToggleButton("Zoom", &State.EnableZoom)) {
                // State.Save();
                if (!State.EnableZoom && Game::HudManager.IsInstanceExists()) {
                    auto hud = Game::HudManager.GetInstance();
                    bool isKillOverlayActive = hud->fields.KillOverlay != NULL &&
                        KillOverlay_get_IsOpen((KillOverlay*)hud->fields.KillOverlay, NULL);
                    if (isKillOverlayActive) State.EnableZoom = true;
                    // the ProgressTracker disappears if you disable zoom during the kill animation
                }
                State.HasRefreshedUI = false;
            }

            ImGui::SameLine(130.f * State.dpiScale);
            SteppedSliderFloat("Escala", &State.CameraHeight, 0.5f, 10.0f, 0.5f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

            if (ToggleButton("Rueda raton para Zoom / Shift + Rueda para Vel. Camara", &State.EnableZoom_ScrollZoom)) {
                State.Save();
            }
            
            if (ToggleButton("Zoom Suave", &State.EnableZoom_SmoothZoom)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Mostrar Sombras con Zoom", &State.EnableZoom_ShowShadows)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (ToggleButton("Boton de Chat Siempre Visible", &State.ChatAlwaysActive)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Permitir Ctrl+(C/V) en Chat", &State.ChatPaste)) { //add copying later
                State.Save();
            }

            if (ToggleButton("Leer Mensajes de Fantasmas", &State.ReadGhostMessages)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Leer y Enviar SickoChat", &State.ReadAndSendSickoChat)) {
                State.Save();
            }
            if (State.ReadAndSendSickoChat) ImGui::Text("¡Envia mensajes de SickoChat escribiendo \"/sc [mensaje]\" en el chat!");

            if (ToggleButton("Mover Boton de Guia de Informacion HUD", &State.MoveMatchInfoGuide)) {
                State.Save();
            }

            if (/*!IsHost() && */State.SafeMode) {
                ImGui::Text("¡Los nombres personalizados solo los ves TU (cliente)!");
            }
            if (ToggleButton("Nombre Personalizado", &State.CustomName)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Nombre Personalizado para Todos", &State.CustomNameForEveryone)) {
                State.Save();
            }

            if (/*IsHost() || */!State.SafeMode) {
                if (ToggleButton("Nombre Personalizado en el Servidor", &State.ServerSideCustomName)) {
                    State.Save();
                }
            }

            if (State.CustomName && ImGui::CollapsingHeader("Opciones de Nombre Personalizado"))
            {
                if (ToggleButton("Cursiva", &State.ItalicName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Subrayado", &State.UnderlineName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Tachado", &State.StrikethroughName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Negrita", &State.BoldName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Sin Salto", &State.NobrName)) {
                    State.Save();
                }

                if (ImGui::ColorEdit4("Color Inicial Degradado", (float*)&State.NameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ImGui::ColorEdit4("Color Final Degradado", (float*)&State.NameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Coloreado", &State.ColoredName)) {
                    State.Save();
                }

                if (ToggleButton("RGB", &State.RgbName)) {
                    State.Save();
                }

                if (CustomListBoxInt("Metodo Degradado", &State.ColorMethod, { "Estatico", "Izquierda a Derecha" }, 80.f * State.dpiScale))
                    State.Save();
                ImGui::SameLine();
                if (CustomListBoxInt("Metodo RGB", &State.RgbMethod, { "Todos a la Vez", "Izquierda a Derecha" }, 80.f * State.dpiScale))
                    State.Save();

                if (ToggleButton("Habilitar Prefijo y Sufijo", &State.UsePrefixAndSuffix)) State.Save();
                if (ToggleButton("Salto de Linea en Prefijo/Sufijo", &State.PrefixAndSuffixNewLines)) State.Save();

                InputString("Prefijo", &State.NamePrefix);
                InputString("Sufijo", &State.NameSuffix);

                if (ToggleButton("Fuente", &State.Font)) {
                    State.Save();
                }
                if (State.Font) {
                    ImGui::SameLine();
                    if (CustomListBoxInt(" ", &State.FontType, FONTS, 160.f * State.dpiScale)) {
                        State.Save();
                    }
                }

                if (ToggleButton("Tamano", &State.ResizeName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Tamano de Letra", &State.NameSize);

                if (ToggleButton("Indent", &State.IndentName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Name Indent", &State.NameIndent);

                ToggleButton("Cspace", &State.CspaceName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Cspace", &State.NameCspace);

                ToggleButton("Mspace", &State.MspaceName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Mspace", &State.NameMspace);

                ToggleButton("Voffset", &State.VoffsetName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Voffset", &State.NameVoffset);
                if (ToggleButton("Rotate", &State.RotateName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Rotation Angle", &State.NameRotate);
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            }

            if (ToggleButton("Revelar Roles", &State.RevealRoles)) {
                State.Save();
                State.MIG_ThemeChanged = true;
            }
            ImGui::SameLine();
            if (ToggleButton("Traducir Nombres de Roles", &State.LocalizeRoleNames))
            {
                if (State.LocalizeRoleNames) State.AbbreviatedRoleNames = false;
                State.Save();
            }
            if (!State.LocalizeRoleNames) ImGui::SameLine();
            if (!State.LocalizeRoleNames && ToggleButton("Abreviar Nombres de Roles", &State.AbbreviatedRoleNames))
            {
                State.Save();
            }

            if (ToggleButton("Punto de Color del Jugador en Nombres", &State.PlayerColoredDots))
            {
                State.Save();
                State.MIG_ThemeChanged = true;
            }

            if (ToggleButton("Ver Info de Jugadores en Sala", &State.ShowPlayerInfo))
            {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ver Info de la Sala", &State.ShowLobbyInfo))
            {
                State.Save();
            }

            if (ToggleButton("Ocultar Info de Lista Blanca", &State.HideWhitelistedPlayerInfo))
            {
                State.Save();
            }

            if (ToggleButton("Revelar Votos", &State.RevealVotes)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Revelar Votos Anonimos", &State.RevealAnonymousVotes)) {
                State.Save();
                RevealAnonymousVotes();
            }

            if (ToggleButton("Ver Fantasmas", &State.ShowGhosts)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ver Phantoms (Invisibles)", &State.ShowPhantoms)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ver Jugadores en Alcantarillas", &State.ShowPlayersInVents)) {
                State.Save();
            }

            if (ToggleButton("Ver Protecciones de Angel", &State.ShowProtections))
            {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ver Enfriamiento de Asesinato", &State.ShowKillCD)) {
                State.Save();
            }

            if (ToggleButton("Desactivar Animacion de Muerte", &State.DisableKillAnimation)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Desactivar Musica de Sala", &State.DisableLobbyMusic)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Texto de Ping Clasico", &State.OldStylePingText)) State.Save();

            if (ToggleButton("Mostrar Anfitrion", &State.ShowHost)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Mostrar Votos de Expulsion", &State.ShowVoteKicks)) {
                State.Save();
            }

            if (ToggleButton("Mostrar Enfriamiento de Chat", &State.ShowChatTimer)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Extender Limite de Letras en Chat", &State.ExtendChatLimit)) {
                State.Save();
            }

            /*if (ToggleButton("Extend Chat History", &State.ExtendChatHistory)) {
                State.Save();
            }*/

            /*if (ToggleButton("Change Body Type", &State.ChangeBodyType)) {
                State.Save();
            }
            if (State.ChangeBodyType) {
                ImGui::SameLine();
                if (CustomListBoxInt("Type", &State.BodyType, BODYTYPES, 75.f * State.dpiScale))
                    State.Save();
            }*/

            if (State.InMeeting && AnimatedButton("Salir de la Reunion"))
            {
                if (IsHost()) State.rpcQueue.push(new RpcEndMeeting());
                else State.rpcQueue.push(new EndMeeting());
            }
        }

        if (openUtils) {
            if (ToggleButton("Desbloquear Alcantarillas (Vents)", &State.UnlockVents)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Moverse en Alcantarilla y Transformacion", &State.MoveInVentAndShapeshift)) {
                if (*Game::pLocalPlayer == NULL) State.Save();
                else if (!State.MoveInVentAndShapeshift && (State.InMeeting || (*Game::pLocalPlayer)->fields.inVent)) {
                    (*Game::pLocalPlayer)->fields.moveable = false;
                    State.Save();
                }
            }
            ImGui::SameLine();
            if (ToggleButton("Siempre Moverse", &State.AlwaysMove)) {
                State.Save();
            }

            if (ToggleButton("Inmunidad a Asesinatos", &State.KillImmunity)) {
                SendKillImmuneToggle(State.KillImmunity);
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Habilidades Ignoran Sabotaje Comms", &State.RolesBypassCommsSabotage)) {
                State.Save();
            }

            /*if (ToggleButton("No Shapeshift Animation", &State.AnimationlessShapeshift)) {
                State.Save();
            }
            ImGui::SameLine();*/
            if (ToggleButton("Copiar Codigo de Sala al Desconectar", &State.AutoCopyLobbyCode)) {
                State.Save();
            }

            if (ToggleButton("Atravesar Paredes (NoClip)", &State.NoClip)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Sin Animacion de Buscador", &State.NoSeekerAnim)) State.Save();

            /*if (ToggleButton("Kill Other Impostors", &State.KillImpostors)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Infinite Kill Range", &State.InfiniteKillRange)) {
                State.Save();
            }*/

            if (ToggleButton("Better Chat Notifications", &State.BetterChatNotifications)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Better Lobby Code Input", &State.BetterLobbyCodeInput)) {
                State.Save();
            }
            
            if (ToggleButton("Mejores Sonidos de Mensajes", &State.BetterMessageSounds)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Notificaciones Extendidas", &State.ExtendedNotifications)) {
                State.Save();
            }

            if (ToggleButton("Auto-Reconectar al Terminar Partida", &State.AutoRejoin)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Auto-Reconectar si te Expulsan", &State.AutoRejoinOnKick)) {
                State.Save();
            }

            if (ToggleButton("Desactivar Animacion de Silencio", &State.DisableShushAnimation)) {
                State.Save();
            }

            if (ToggleButton("Controlar Mascota", &State.ControlPet)) {
                if (*Game::pLocalPlayer == nullptr || (!IsInGame() && !IsInLobby())) State.ControlPet = false;
                if (!State.ControlPet) State.DisableControlPetHand = true;
            }
            ImGui::SameLine();
            /*if (ToggleButton("Show Hand While Controlling Pet", &State.ShowPetHand)) {
                State.Save();
            }*/

            /*if (ToggleButton("Autokill", &State.AutoKill)) {
                State.Save();
            }*/

            if (ToggleButton("Reportar Cuerpo al Asesinar", &State.ReportOnMurder)) {
                State.Save();
            }
            if (State.ReportOnMurder) {
                ImGui::SameLine();
                if (ToggleButton("Evitar Auto-Reporte", &State.PreventSelfReport)) {
                    State.Save();
                }
            }
            /*ImGui::SameLine();
            if (ToggleButton("Always Use Kill Exploit", &State.AlwaysUseKillExploit)) {
                State.Save();
            }*/

            if (ToggleButton("Fingir Estar Vivo", &State.FakeAlive)) {
                State.Save();
            }
            ImGui::SameLine();
            if (((IsHost() && IsInGame()) || !State.SafeMode) && ToggleButton(IsHost() ? "Modo Dios" : "Proteccion Visual", &State.GodMode))
                State.Save();

            if (ToggleButton("(Shift/Ctrl + Clic Derecho) Teletransportar", &State.ShiftRightClickTP)) {
                State.Save();
            }
            if (!State.SafeMode) ImGui::SameLine();
            if (!State.SafeMode && ToggleButton("Mantener ALT para Teletransportar a Todos", &State.TeleportEveryone)) {
                State.Save();
            }
            if (ToggleButton((State.SafeMode ? "Girar a Todos (Solo Visual)" : "Girar a Todos"), &State.RotateEveryone)) {
                State.Save();
            }
            if (!State.SafeMode) ImGui::SameLine();
            if (!State.SafeMode && State.RotateEveryone && ToggleButton("Giro en Servidor", &State.RotateServerSide)) {
                State.Save();
            }
            ImGui::InputFloat("Radio de Giro", &State.RotateRadius, 0.0f, 0.0f, "%.2f m");

            ImGui::InputFloat("Coordenada X", &State.xCoordinate, 0.0f, 0.0f, "%.4f X");

            ImGui::InputFloat("Coordenada Y", &State.yCoordinate, 0.0f, 0.0f, "%.4f Y");

            if (ToggleButton("Teletransporte Relativo", &State.RelativeTeleport)) {
                State.Save();
            }
            if (IsInGame() || IsInLobby())
                ImGui::SameLine();
            if ((IsInGame() || IsInLobby()) && AnimatedButton("Obtener Posicion Actual"))
            {
                Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                State.xCoordinate = position.x;
                State.yCoordinate = position.y;
            }
            if (IsInGame() || IsInLobby())
                ImGui::SameLine();

            if ((IsInGame() || IsInLobby()) && AnimatedButton("Teletransportarse A"))
            {
                Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                Vector2 target = { (State.RelativeTeleport ? position.x : 0.f) + State.xCoordinate, (State.RelativeTeleport ? position.y : 0.f) + State.yCoordinate };
                if (IsInGame()) {
                    State.rpcQueue.push(new RpcSnapTo(target));
                }
                else if (IsInLobby()) {
                    State.lobbyRpcQueue.push(new RpcSnapTo(target));
                }
            }
            if (!State.SafeMode && (IsInGame() || IsInLobby())) {
                ImGui::SameLine();
                if (AnimatedButton("Teletransportar a Todos A"))
                {
                    Vector2 position = GetTrueAdjustedPosition(*Game::pLocalPlayer);
                    Vector2 target = { (State.RelativeTeleport ? position.x : 0.f) + State.xCoordinate, (State.RelativeTeleport ? position.y : 0.f) + State.yCoordinate };
                    std::queue<RPCInterface*>* queue = nullptr;
                    if (IsInGame())
                        queue = &State.rpcQueue;
                    else if (IsInLobby())
                        queue = &State.lobbyRpcQueue;
                    for (auto player : GetAllPlayerControl()) {
                        queue->push(new RpcForceSnapTo(player, target));
                    }
                }
            }

            ColorMapping FAKEROLE_NAMES_COLOR[] = {
                {"Crewmate",		State.CrewmateColor},
                {"Impostor",		State.ImpostorColor},
                {"Scientist",		State.ScientistColor},
                {"Engineer",		State.EngineerColor},
                {"Guardian Angel",	State.GuardianAngelColor},
                {"Shapeshifter",	State.ShapeshifterColor},
                {"Crewmate Ghost",  State.CrewmateGhostColor},
                {"Impostor Ghost",	State.ImpostorGhostColor},
                {"Noisemaker",		State.NoisemakerColor},
                {"Phantom",			State.PhantomColor},
                {"Tracker",			State.TrackerColor},
                {"Detective",		State.DetectiveColor},
                {"Viper",			State.ViperColor},
                {"Judge",           State.JudgeColor},
                {"Influencer",      State.InfluencerColor},
            }; // needs to be updated every render

            if (CustomListBoxIntColored("Seleccionar Rol", &State.FakeRole, FAKEROLES, 100.0f * State.dpiScale, ImVec4(1.f, 1.f, 1.f, 0.f), 0, " ", FAKEROLE_NAMES_COLOR, IM_ARRAYSIZE(FAKEROLE_NAMES_COLOR))) {
                // for some reason, detective is 12 (0x0c) instead of 11, viper is 18 (0x12) instead of 12, and influencer (SpiritGuide) is 21 (0x15) instead of 20
                if (State.FakeRole >= 14) State.FakeRoleId = State.FakeRole + 7;
                else if (State.FakeRole >= 12) State.FakeRoleId = State.FakeRole + 6;
                else if (State.FakeRole == 11) State.FakeRoleId = State.FakeRole + 1;
                else State.FakeRoleId = State.FakeRole;
                State.Save();
            }
            ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && (IsInGame() || IsInLobby()) && AnimatedButton("Fijar Rol")) {
                // State.FakeRole = std::clamp(State.FakeRole, 0, 10);
                if (IsInGame())
                    State.rpcQueue.push(new RpcSetRole(*Game::pLocalPlayer, RoleTypes__Enum(State.FakeRoleId)));
                else if (IsInLobby())
                    State.lobbyRpcQueue.push(new RpcSetRole(*Game::pLocalPlayer, RoleTypes__Enum(State.FakeRoleId)));
            }
            if (IsHost() || !State.SafeMode) ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && (IsInGame() || IsInLobby()) && AnimatedButton("Fijar para Todos")) {
                // State.FakeRole = std::clamp(State.FakeRole, 0, 10);
                if (IsInGame()) {
                    for (auto player : GetAllPlayerControl())
                        State.rpcQueue.push(new RpcSetRole(player, RoleTypes__Enum(State.FakeRoleId)));
                }
                else if (IsInLobby()) {
                    for (auto player : GetAllPlayerControl())
                        State.lobbyRpcQueue.push(new RpcSetRole(player, RoleTypes__Enum(State.FakeRoleId)));
                }
            }
            bool roleAllowed = false;
            switch (State.FakeRoleId) {
            case (int)RoleTypes__Enum::Crewmate:
            case (int)RoleTypes__Enum::Engineer:
            case (int)RoleTypes__Enum::Scientist:
            case (int)RoleTypes__Enum::Tracker:
            case (int)RoleTypes__Enum::Detective:
            case (int)RoleTypes__Enum::CrewmateGhost:
            case (int)RoleTypes__Enum::ImpostorGhost:
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Noisemaker:
                if (State.RealRole != RoleTypes__Enum::Noisemaker) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::GuardianAngel:
                if (!IsHost() && State.SafeMode && State.RealRole != RoleTypes__Enum::GuardianAngel) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Judge:
                if (State.SafeMode && State.RealRole != RoleTypes__Enum::Judge) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Impostor:
                if (!IsHost() && State.SafeMode && State.RealRole != RoleTypes__Enum::Impostor && State.RealRole != RoleTypes__Enum::Shapeshifter && State.RealRole != RoleTypes__Enum::Phantom && State.RealRole != RoleTypes__Enum::Viper) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Shapeshifter:
                if (State.SafeMode && State.RealRole != RoleTypes__Enum::Shapeshifter) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Phantom:
                if (State.RealRole != RoleTypes__Enum::Phantom) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::Viper:
                if (State.RealRole != RoleTypes__Enum::Viper) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            case (int)RoleTypes__Enum::SpiritGuide:
                if (State.RealRole != RoleTypes__Enum::SpiritGuide) {
                    roleAllowed = false;
                    break;
                }
                roleAllowed = true;
                break;
            default:
                roleAllowed = false;
                break;
            }
            if ((IsInGame() || IsInLobby()) && (roleAllowed || (IsHost() || !State.SafeMode)) && AnimatedButton("Fijar Rol Falso")) {
                if (IsInGame())
                    State.rpcQueue.push(new SetRole(RoleTypes__Enum(State.FakeRoleId)));
                else if (IsInLobby())
                    State.lobbyRpcQueue.push(new SetRole(RoleTypes__Enum(State.FakeRoleId)));
            }
            ImGui::SameLine();
            if (ToggleButton("Fijar Rol Falso Automaticamente", &State.AutoFakeRole)) {
                State.Save();
            }
            /*if (IsInLobby() || IsInGame()) {
                ImGui::SameLine();
                std::string roleText = FAKEROLES[int(State.RealRole)];
                ImGui::Text(("Real Role: " + roleText).c_str());
            }*/

            if (!State.SafeMode) {
                if (ToggleButton("Desbloquear Boton de Asesinar", &State.UnlockKillButton)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Asesinar Estando Invisible", &State.KillInVanish)) {
                    State.Save();
                }
                if (ToggleButton("Ignorar Proteccion de Angel Guardian", &State.BypassAngelProt)) {
                    State.Save();
                }
            }
        }

        if (openRoles) {
            if (ToggleButton("Reuniones de Emergencia Infinitas", &State.InfiniteMeetings)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Sin Cooldown en Escalera/Tirolesa", &State.NoLadderZiplineCooldown)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.EngineerColor, "Ingeniero");
            if (ToggleButton("Sin Cooldown en Alcantarilla", &State.Engineer_NoVentCooldown)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Tiempo Infinito en Alcantarilla", &State.Engineer_InfiniteVentTime)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.ScientistColor, "Cientifico");
            if (ToggleButton("Sin Cooldown en Vitales", &State.Scientist_NoVitalsCooldown)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Bateria Infinita", &State.Scientist_InfiniteBattery)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.TrackerColor, "Rastreador");
            if (ToggleButton("Sin Cooldown en Rastreo", &State.Tracker_NoTrackingCooldown)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Rastreo Infinito", &State.Tracker_InfiniteTracking)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.DetectiveColor, "Detective");
            if (ToggleButton("Sin Cooldown en Interrogar", &State.Detective_NoInterrogateCooldown)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.JudgeColor, "Juez");
            if (ToggleButton("Sin Requisito de Tareas", &State.Judge_NoTaskRequirement)) State.Save();
            if (!State.SafeMode) {
                ImGui::SameLine();
                if (ToggleButton("Anulaciones Infinitas", &State.Judge_InfiniteOverrules)) State.Save();
            }
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            if (IsHost() || !State.SafeMode) {
                ImGui::TextColored(State.GuardianAngelColor, "Angel Guardian");
                if (ToggleButton("Sin Cooldown en Proteger", &State.GuardianAngel_NoProtectCooldown)) State.Save();
                ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            }

            ImGui::TextColored(State.InfluencerColor, "Influencer");
            if (ToggleButton("Sin Cooldown en Recargar", &State.Influencer_NoRefreshCooldown)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.ImpostorColor, "Impostor");
            if (ToggleButton("Matar a Otros Impostores", &State.KillImpostors)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Alcance de Asesinato Infinito", &State.InfiniteKillRange)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Hacer Tareas como Impostor", &State.DoTasksAsImpostor)) State.Save();

            if (IsHost() && ToggleButton("Sin Cooldown de Asesinato", &State.Impostor_NoKillCooldown)) State.Save();
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            ImGui::TextColored(State.ShapeshifterColor, "Cambiaformas");
            if (ToggleButton("Sin Animacion de Transformacion", &State.AnimationlessShapeshift)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("Duracion Infinita de Transformacion", &State.Shapeshifter_InfiniteShapeshiftDuration)) State.Save();
        }

        if (openRandomizers) {
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            if (ToggleButton("Ciclo de Apariencia (Cycler)", &State.Cycler)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Ciclar en Reunion", &State.CycleInMeeting)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton(State.SafeMode ? "Ciclar Atuendos de Jugadores" : "Ciclar Entre Jugadores", &State.CycleBetweenPlayers)) {
                State.Save();
            }

            if (SteppedSliderFloat("Temporizador de Ciclo", &State.CycleTimer, 0.2f, 1.f, 0.02f, "%.2fs", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevCycleTimer = State.CycleTimer;
                State.CycleDuration = State.CycleTimer * 50;
            }

            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ImGui::CollapsingHeader("Opciones de Ciclo")) {
                ImGui::Dummy(ImVec2(4, 2)* State.dpiScale);
                if (!State.SafeMode) {
                    if (ToggleButton("Ciclar Nombre", &State.CycleName)) {
                        State.Save();
                    }

                    ImGui::SameLine(120.0f * State.dpiScale);
                }
                if (ToggleButton("Ciclar Color", &State.RandomColor)) {
                    State.Save();
                }

                ImGui::SameLine(!State.SafeMode ? (240.0f * State.dpiScale) : (120.0f * State.dpiScale));
                if (ToggleButton("Ciclar Sombrero", &State.RandomHat)) {
                    State.Save();
                }
                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Ciclar Placa", &State.RandomNamePlate)) {
                    State.Save();
                }
                if (ToggleButton("Ciclar Visor", &State.RandomVisor)) {
                    State.Save();
                }

                ImGui::SameLine(120.0f * State.dpiScale);
                if (ToggleButton("Ciclar Traje", &State.RandomSkin)) {
                    State.Save();
                }

                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Ciclar Mascota", &State.RandomPet)) {
                    State.Save();
                }

                if (IsHost() || !State.SafeMode) {
                    if (ToggleButton(IsHost() ? "Ciclar para Todos (Solo Color)" : "Ciclar para Todos", &State.CycleForEveryone)) {
                        State.Save();
                    }
                }
            }

            ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

            if (!State.SafeMode && ImGui::CollapsingHeader("Opciones de Nombres del Ciclo")) {
                if (CustomListBoxInt("Generacion de Nombres", &State.cyclerNameGeneration, NAMEGENERATION, 75 * State.dpiScale)) {
                    State.Save();
                }
                if (State.cyclerNameGeneration == 2) {
                    if (State.cyclerUserNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sin nombres en lista; se usara combinacion de palabras.");
                    static std::string newName = "";
                    InputString("Nuevo Nombre", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::SameLine();
                    if (AnimatedButton("Agregar Nombre")) {
                        State.cyclerUserNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }
                    if (!(IsHost() || !State.SafeMode) && !IsNameValid(newName)) {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre bloqueado por anticheat.");
                    }
                    if (!State.cyclerUserNames.empty()) {
                        static int selectedNameIndex = 0;
                        selectedNameIndex = std::clamp(selectedNameIndex, 0, (int)State.cyclerUserNames.size() - 1);
                        std::vector<const char*> nameVector(State.cyclerUserNames.size(), nullptr);
                        for (size_t i = 0; i < State.cyclerUserNames.size(); i++) {
                            nameVector[i] = State.cyclerUserNames[i].c_str();
                        }
                        CustomListBoxInt("Nombre a Eliminar", &selectedNameIndex, nameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Eliminar"))
                            State.cyclerUserNames.erase(State.cyclerUserNames.begin() + selectedNameIndex);
                    }
                }
            }

            if (ImGui::CollapsingHeader("Confusor", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Dummy(ImVec2(4, 2) * State.dpiScale);
                if (ToggleButton("Confusor (Aleatorizar Apariencia)", &State.confuser)) {
                    State.Save();
                }

                ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                if ((IsInGame() || IsInLobby()) && AnimatedButton("Confundir Ahora")) {
                    ControlAppearance(true);
                    Toasts::AddToast("Confusor", "¡Atuendo aleatorizado!", ImVec4(0.f, 1.f, 1.f, 1.f));
                }
                if (IsInGame() || IsInLobby()) {
                    if (IsHost() || !State.SafeMode)
                        ImGui::SameLine();
                }
                if ((IsInGame() || IsInLobby()) && !State.SafeMode && AnimatedButton("Aleatorizar a Todos")) {
                    std::queue<RPCInterface*>* queue = nullptr;
                    if (IsInGame())
                        queue = &State.rpcQueue;
                    else if (IsInLobby())
                        queue = &State.lobbyRpcQueue;
                    std::vector availableHats = { "hat_NoHat", "hat_AbominalHat", "hat_anchor", "hat_antenna", "hat_Antenna_Black", "hat_arrowhead", "hat_Astronaut-Blue", "hat_Astronaut-Cyan", "hat_Astronaut-Orange", "hat_astronaut", "hat_axe", "hat_babybean", "hat_Baguette", "hat_BananaGreen", "hat_BananaPurple", "hat_bandanaWBY", "hat_Bandana_Blue", "hat_Bandana_Green", "hat_Bandana_Pink", "hat_Bandana_Red", "hat_Bandana_White", "hat_Bandana_Yellow", "hat_baseball_Black", "hat_baseball_Green", "hat_baseball_Lightblue", "hat_baseball_LightGreen", "hat_baseball_Lilac", "hat_baseball_Orange", "hat_baseball_Pink", "hat_baseball_Purple", "hat_baseball_Red", "hat_baseball_White", "hat_baseball_Yellow", "hat_Basketball", "hat_bat_crewcolor", "hat_bat_green", "hat_bat_ice", "hat_beachball", "hat_Beanie_Black", "hat_Beanie_Blue", "hat_Beanie_Green", "hat_Beanie_Lightblue", "hat_Beanie_LightGreen", "hat_Beanie_LightPurple", "hat_Beanie_Pink", "hat_Beanie_Purple", "hat_Beanie_White", "hat_Beanie_Yellow", "hat_bearyCold", "hat_bone", "hat_Bowlingball", "hat_brainslug", "hat_BreadLoaf", "hat_bucket", "hat_bucketHat", "hat_bushhat", "hat_Butter", "hat_caiatl", "hat_caitlin", "hat_candycorn", "hat_captain", "hat_cashHat", "hat_cat_grey", "hat_cat_orange", "hat_cat_pink", "hat_cat_snow", "hat_chalice", "hat_cheeseBleu", "hat_cheeseMoldy", "hat_cheeseSwiss", "hat_ChefWhiteBlue", "hat_cherryOrange", "hat_cherryPink", "hat_Chocolate", "hat_chocolateCandy", "hat_chocolateMatcha", "hat_chocolateVanillaStrawb", "hat_clagger", "hat_clown_purple", "hat_comper", "hat_croissant", "hat_crownBean", "hat_crownDouble", "hat_crownTall", "hat_CuppaJoe", "hat_Deitied", "hat_devilhorns_black", "hat_devilhorns_crewcolor", "hat_devilhorns_green", "hat_devilhorns_murky", "hat_devilhorns_white", "hat_devilhorns_yellow", "hat_Doc_black", "hat_Doc_Orange", "hat_Doc_Purple", "hat_Doc_Red", "hat_Doc_White", "hat_Dodgeball", "hat_Dorag_Black", "hat_Dorag_Desert", "hat_Dorag_Jungle", "hat_Dorag_Purple", "hat_Dorag_Sky", "hat_Dorag_Snow", "hat_Dorag_Yellow", "hat_doubletophat", "hat_DrillMetal", "hat_DrillStone", "hat_DrillWood", "hat_EarmuffGreen", "hat_EarmuffsPink", "hat_EarmuffsYellow", "hat_EarnmuffBlue", "hat_eggGreen", "hat_eggYellow", "hat_enforcer", "hat_erisMorn", "hat_fairywings", "hat_fishCap", "hat_fishhed", "hat_fishingHat", "hat_flowerpot", "hat_frankenbolts", "hat_frankenbride", "hat_fungleFlower", "hat_geoff", "hat_glowstick", "hat_glowstickCyan", "hat_glowstickOrange", "hat_glowstickPink", "hat_glowstickPurple", "hat_glowstickYellow", "hat_goggles", "hat_Goggles_Black", "hat_Goggles_Chrome", "hat_GovtDesert", "hat_GovtHeadset", "hat_halospartan", "hat_hardhat", "hat_Hardhat_black", "hat_Hardhat_Blue", "hat_Hardhat_Green", "hat_Hardhat_Orange", "hat_Hardhat_Pink", "hat_Hardhat_Purple", "hat_Hardhat_Red", "hat_Hardhat_White", "hat_HardtopHat", "hat_headslug_Purple", "hat_headslug_Red", "hat_headslug_White", "hat_headslug_Yellow", "hat_Heart", "hat_heim", "hat_Herohood_Black", "hat_Herohood_Blue", "hat_Herohood_Pink", "hat_Herohood_Purple", "hat_Herohood_Red", "hat_Herohood_Yellow", "hat_hl_fubuki", "hat_hl_gura", "hat_hl_korone", "hat_hl_marine", "hat_hl_mio", "hat_hl_moona", "hat_hl_okayu", "hat_hl_pekora", "hat_hl_risu", "hat_hl_watson", "hat_hunter", "hat_IceCreamMatcha", "hat_IceCreamMint", "hat_IceCreamNeo", "hat_IceCreamStrawberry", "hat_IceCreamUbe", "hat_IceCreamVanilla", "hat_Igloo", "hat_Janitor", "hat_jayce", "hat_jinx", "hat_killerplant", "hat_lilShroom", "hat_maraSov", "hat_mareLwyd", "hat_military", "hat_MilitaryWinter", "hat_MinerBlack", "hat_MinerYellow", "hat_mira_bush", "hat_mira_case", "hat_mira_cloud", "hat_mira_flower", "hat_mira_flower_red", "hat_mira_gem", "hat_mira_headset_blue", "hat_mira_headset_pink", "hat_mira_headset_yellow", "hat_mira_leaf", "hat_mira_milk", "hat_mira_sign_blue", "hat_mohawk_bubblegum", "hat_mohawk_bumblebee", "hat_mohawk_purple_green", "hat_mohawk_rainbow", "hat_mummy", "hat_mushbuns", "hat_mushroomBeret", "hat_mysteryBones", "hat_NewYear2023", "hat_OrangeHat", "hat_osiris", "hat_pack01_Astronaut0001", "hat_pack02_Tengallon0001", "hat_pack02_Tengallon0002", "hat_pack03_Stickynote0004", "hat_pack04_Geoffmask0001", "hat_pack06holiday_candycane0001", "hat_PancakeStack", "hat_paperhat", "hat_Paperhat_Black", "hat_Paperhat_Blue", "hat_Paperhat_Cyan", "hat_Paperhat_Lightblue", "hat_Paperhat_Pink", "hat_Paperhat_Yellow", "hat_papermask", "hat_partyhat", "hat_pickaxe", "hat_Pineapple", "hat_PizzaSliceHat", "hat_pk01_BaseballCap", "hat_pk02_Crown", "hat_pk02_Eyebrows", "hat_pk02_HaloHat", "hat_pk02_HeroCap", "hat_pk02_PipCap", "hat_pk02_PlungerHat", "hat_pk02_ScubaHat", "hat_pk02_StickminHat", "hat_pk02_StrawHat", "hat_pk02_TenGallonHat", "hat_pk02_ThirdEyeHat", "hat_pk02_ToiletPaperHat", "hat_pk02_Toppat", "hat_pk03_Fedora", "hat_pk03_Goggles", "hat_pk03_Headphones", "hat_pk03_Security1", "hat_pk03_StrapHat", "hat_pk03_Traffic", "hat_pk04_Antenna", "hat_pk04_Archae", "hat_pk04_Balloon", "hat_pk04_Banana", "hat_pk04_Bandana", "hat_pk04_Beanie", "hat_pk04_Bear", "hat_pk04_BirdNest", "hat_pk04_CCC", "hat_pk04_Chef", "hat_pk04_DoRag", "hat_pk04_Fez", "hat_pk04_GeneralHat", "hat_pk04_HunterCap", "hat_pk04_JungleHat", "hat_pk04_MinerCap", "hat_pk04_MiniCrewmate", "hat_pk04_Pompadour", "hat_pk04_RamHorns", "hat_pk04_Slippery", "hat_pk04_Snowman", "hat_pk04_Vagabond", "hat_pk04_WinterHat", "hat_pk05_Burthat", "hat_pk05_Cheese", "hat_pk05_cheesetoppat", "hat_pk05_Cherry", "hat_pk05_davehat", "hat_pk05_Egg", "hat_pk05_Ellie", "hat_pk05_EllieToppat", "hat_pk05_Ellryhat", "hat_pk05_Fedora", "hat_pk05_Flamingo", "hat_pk05_FlowerPin", "hat_pk05_GeoffreyToppat", "hat_pk05_Helmet", "hat_pk05_HenryToppat", "hat_pk05_Macbethhat", "hat_pk05_Plant", "hat_pk05_RHM", "hat_pk05_Svenhat", "hat_pk05_Wizardhat", "hat_pk06_Candycanes", "hat_pk06_ElfHat", "hat_pk06_Lights", "hat_pk06_Present", "hat_pk06_Reindeer", "hat_pk06_Santa", "hat_pk06_Snowman", "hat_pk06_tree", "hat_pkHW01_BatWings", "hat_pkHW01_CatEyes", "hat_pkHW01_Horns", "hat_pkHW01_Machete", "hat_pkHW01_Mohawk", "hat_pkHW01_Pirate", "hat_pkHW01_PlagueHat", "hat_pkHW01_Pumpkin", "hat_pkHW01_ScaryBag", "hat_pkHW01_Witch", "hat_pkHW01_Wolf", "hat_Plunger_Blue", "hat_Plunger_Yellow", "hat_police", "hat_Ponytail", "hat_Pot", "hat_Present", "hat_Prototype", "hat_pusheenGreyHat", "hat_PusheenicornHat", "hat_pusheenMintHat", "hat_pusheenPinkHat", "hat_pusheenPurpleHat", "hat_pusheenSitHat", "hat_pusheenSleepHat", "hat_pyramid", "hat_rabbitEars", "hat_Ramhorn_Black", "hat_Ramhorn_Red", "hat_Ramhorn_White", "hat_ratchet", "hat_Records", "hat_RockIce", "hat_RockLava", "hat_Rubberglove", "hat_Rupert", "hat_russian", "hat_saint14", "hat_sausage", "hat_savathun", "hat_schnapp", "hat_screamghostface", "hat_Scrudge", "hat_sharkfin", "hat_shaxx", "hat_shovel", "hat_SlothHat", "hat_SnowbeanieGreen", "hat_SnowbeanieOrange", "hat_SnowBeaniePurple", "hat_SnowbeanieRed", "hat_Snowman", "hat_Soccer", "hat_Sorry", "hat_starBalloon", "hat_starhorse", "hat_Starless", "hat_StarTopper", "hat_stethescope", "hat_StrawberryLeavesHat", "hat_TenGallon_Black", "hat_TenGallon_White", "hat_ThomasC", "hat_tinFoil", "hat_titan", "hat_ToastButterHat", "hat_tombstone", "hat_tophat", "hat_ToppatHair", "hat_towelwizard", "hat_Traffic_Blue", "hat_traffic_purple", "hat_Traffic_Red", "hat_Traffic_Yellow", "hat_Unicorn", "hat_vi", "hat_viking", "hat_Visor", "hat_Voleyball", "hat_w21_candycane_blue", "hat_w21_candycane_bubble", "hat_w21_candycane_chocolate", "hat_w21_candycane_mint", "hat_w21_elf_pink", "hat_w21_elf_swe", "hat_w21_gingerbread", "hat_w21_holly", "hat_w21_krampus", "hat_w21_lights_white", "hat_w21_lights_yellow", "hat_w21_log", "hat_w21_mistletoe", "hat_w21_mittens", "hat_w21_nutcracker", "hat_w21_pinecone", "hat_w21_present_evil", "hat_w21_present_greenyellow", "hat_w21_present_redwhite", "hat_w21_present_whiteblue", "hat_w21_santa_evil", "hat_w21_santa_green", "hat_w21_santa_mint", "hat_w21_santa_pink", "hat_w21_santa_white", "hat_w21_santa_yellow", "hat_w21_snowflake", "hat_w21_snowman", "hat_w21_snowman_evil", "hat_w21_snowman_greenred", "hat_w21_snowman_redgreen", "hat_w21_snowman_swe", "hat_w21_winterpuff", "hat_wallcap", "hat_warlock", "hat_whitetophat", "hat_wigJudge", "hat_wigTall", "hat_WilfordIV", "hat_Winston", "hat_WinterGreen", "hat_WinterHelmet", "hat_WinterRed", "hat_WinterYellow", "hat_witch_green", "hat_witch_murky", "hat_witch_pink", "hat_witch_white", "hat_wolf_grey", "hat_wolf_murky", "hat_Zipper" };
                    std::vector availableSkins = { "skin_None", "skin_Abominalskin", "skin_ApronGreen", "skin_Archae", "skin_Astro", "skin_Astronaut-Blueskin", "skin_Astronaut-Cyanskin", "skin_Astronaut-Orangeskin", "skin_Bananaskin", "skin_benoit", "skin_Bling", "skin_BlueApronskin", "skin_BlueSuspskin", "skin_Box1skin", "skin_BubbleWrapskin", "skin_Burlapskin", "skin_BushSign1skin", "skin_Bushskin", "skin_BusinessFem-Aquaskin", "skin_BusinessFem-Tanskin", "skin_BusinessFemskin", "skin_caitlin", "skin_Capt", "skin_CCC", "skin_ChefBlackskin", "skin_ChefBlue", "skin_ChefRed", "skin_clown", "skin_D2Cskin", "skin_D2Hunter", "skin_D2Osiris", "skin_D2Saint14", "skin_D2Shaxx", "skin_D2Titan", "skin_D2Warlock", "skin_enforcer", "skin_fairy", "skin_FishingSkinskin", "skin_fishmonger", "skin_FishSkinskin", "skin_General", "skin_greedygrampaskin", "skin_halospartan", "skin_Hazmat-Blackskin", "skin_Hazmat-Blueskin", "skin_Hazmat-Greenskin", "skin_Hazmat-Pinkskin", "skin_Hazmat-Redskin", "skin_Hazmat-Whiteskin", "skin_Hazmat", "skin_heim", "skin_hl_fubuki", "skin_hl_gura", "skin_hl_korone", "skin_hl_marine", "skin_hl_mio", "skin_hl_moona", "skin_hl_okayu", "skin_hl_pekora", "skin_hl_risu", "skin_hl_watson", "skin_Horse1skin", "skin_Hotdogskin", "skin_InnerTubeSkinskin", "skin_JacketGreenskin", "skin_JacketPurpleskin", "skin_JacketYellowskin", "skin_Janitorskin", "skin_jayce", "skin_jinx", "skin_LifeVestSkinskin", "skin_Mech", "skin_MechanicRed", "skin_Military", "skin_MilitaryDesert", "skin_MilitarySnowskin", "skin_Miner", "skin_MinerBlackskin", "skin_mummy", "skin_OrangeSuspskin", "skin_PinkApronskin", "skin_PinkSuspskin", "skin_Police", "skin_presentskin", "skin_prisoner", "skin_PrisonerBlue", "skin_PrisonerTanskin", "skin_pumpkin", "skin_PusheenGreyskin", "skin_Pusheenicornskin", "skin_PusheenMintskin", "skin_PusheenPinkskin", "skin_PusheenPurpleskin", "skin_ratchet", "skin_rhm", "skin_RockIceskin", "skin_RockLavaskin", "skin_Sack1skin", "skin_scarfskin", "skin_Science", "skin_Scientist-Blueskin", "skin_Scientist-Darkskin", "skin_screamghostface", "skin_Security", "skin_Skin_SuitRedskin", "skin_Slothskin", "skin_SportsBlueskin", "skin_SportsRedskin", "skin_SuitB", "skin_SuitW", "skin_SweaterBlueskin", "skin_SweaterPinkskin", "skin_Sweaterskin", "skin_SweaterYellowskin", "skin_Tarmac", "skin_ToppatSuitFem", "skin_ToppatVest", "skin_uglysweaterskin", "skin_vampire", "skin_vi", "skin_w21_deer", "skin_w21_elf", "skin_w21_msclaus", "skin_w21_nutcracker", "skin_w21_santa", "skin_w21_snowmate", "skin_w21_tree", "skin_Wall", "skin_Winter", "skin_witch", "skin_YellowApronskin", "skin_YellowSuspskin" };
                    std::vector availableVisors = { "visor_EmptyVisor", "visor_anime", "visor_BaconVisor", "visor_BananaVisor", "visor_beautyMark", "visor_BillyG", "visor_Blush", "visor_Bomba", "visor_BubbleBumVisor", "visor_Candycane", "visor_Carrot", "visor_chimkin", "visor_clownnose", "visor_Crack", "visor_CucumberVisor", "visor_D2CGoggles", "visor_Dirty", "visor_Dotdot", "visor_doubleeyepatch", "visor_eliksni", "visor_erisBandage", "visor_eyeball", "visor_EyepatchL", "visor_EyepatchR", "visor_fishhook", "visor_Galeforce", "visor_heim", "visor_hl_ah", "visor_hl_bored", "visor_hl_hmph", "visor_hl_marine", "visor_hl_nothoughts", "visor_hl_nudge", "visor_hl_smug", "visor_hl_sweepy", "visor_hl_teehee", "visor_hl_wrong", "visor_IceBeard", "visor_IceCreamChocolateVisor", "visor_IceCreamMintVisor", "visor_IceCreamStrawberryVisor", "visor_IceCreamUbeVisor", "visor_is_beard", "visor_JanitorStache", "visor_jinx", "visor_Krieghaus", "visor_Lava", "visor_LolliBlue", "visor_LolliBrown", "visor_LolliOrange", "visor_lollipopCrew", "visor_lollipopLemon", "visor_lollipopLime", "visor_LolliRed", "visor_marshmallow", "visor_masque_blue", "visor_masque_green", "visor_masque_red", "visor_masque_white", "visor_mira_card_blue", "visor_mira_card_red", "visor_mira_glasses", "visor_mira_mask_black", "visor_mira_mask_blue", "visor_mira_mask_green", "visor_mira_mask_purple", "visor_mira_mask_red", "visor_mira_mask_white", "visor_Mouth", "visor_mummy", "visor_PiercingL", "visor_PiercingR", "visor_PizzaVisor", "visor_pk01_AngeryVisor", "visor_pk01_DumStickerVisor", "visor_pk01_FredVisor", "visor_pk01_HazmatVisor", "visor_pk01_MonoclesVisor", "visor_pk01_PaperMaskVisor", "visor_pk01_PlagueVisor", "visor_pk01_RHMVisor", "visor_pk01_Security1Visor", "visor_Plsno", "visor_polus_ice", "visor_pusheenGorgeousVisor", "visor_pusheenKissyVisor", "visor_pusheenKoolKatVisor", "visor_pusheenOmNomNomVisor", "visor_pusheenSmileVisor", "visor_pusheenYaaaaaayVisor", "visor_Reginald", "visor_Rudolph", "visor_savathun", "visor_Scar", "visor_SciGoggles", "visor_shopglasses", "visor_shuttershadesBlue", "visor_shuttershadesLime", "visor_shuttershadesPink", "visor_shuttershadesPurple", "visor_shuttershadesWhite", "visor_shuttershadesYellow", "visor_SkiGoggleBlack", "visor_SKiGogglesOrange", "visor_SkiGogglesWhite", "visor_SmallGlasses", "visor_SmallGlassesBlue", "visor_SmallGlassesRed", "visor_starfish", "visor_Stealthgoggles", "visor_Stickynote_Cyan", "visor_Stickynote_Green", "visor_Stickynote_Orange", "visor_Stickynote_Pink", "visor_Stickynote_Purple", "visor_Straw", "visor_sunscreenv", "visor_teary", "visor_ToastVisor", "visor_tvColorTest", "visor_vr_Vr-Black", "visor_vr_Vr-White", "visor_w21_carrot", "visor_w21_nutstache", "visor_w21_nye", "visor_w21_santabeard", "visor_wash", "visor_WinstonStache" };
                    std::vector availablePets = { "pet_EmptyPet", "pet_Alien", "pet_Bedcrab", "pet_BredPet", "pet_Bush", "pet_Charles", "pet_Charles_Red", "pet_ChewiePet", "pet_clank", "pet_coaltonpet", "pet_Creb", "pet_Crewmate", "pet_Cube", "pet_D2GhostPet", "pet_D2PoukaPet", "pet_D2WormPet", "pet_Doggy", "pet_Ellie", "pet_frankendog", "pet_GuiltySpark", "pet_HamPet", "pet_Hamster", "pet_HolidayHamPet", "pet_Lava", "pet_nuggetPet", "pet_Pip", "pet_poro", "pet_Pusheen", "pet_Robot", "pet_Snow", "pet_Squig", "pet_Stickmin", "pet_Stormy", "pet_test", "pet_UFO", "pet_YuleGoatPet" };
                    std::vector availableNamePlates = { "nameplate_NoPlate", "nameplate_cliffs", "nameplate_grill", "nameplate_plant", "nameplate_sandcastle", "nameplate_zipline", "nameplate_pusheen_01", "nameplate_pusheen_02", "nameplate_pusheen_03", "nameplate_pusheen_04", "nameplate_flagAro", "nameplate_flagMlm", "nameplate_hunter", "nameplate_Polus_DVD", "nameplate_Polus_Ground", "nameplate_Polus_Lava", "nameplate_Polus_Planet", "nameplate_Polus_Snow", "nameplate_Polus_SpecimenBlue", "nameplate_Polus_SpecimenGreen", "nameplate_Polus_SpecimenPurple", "nameplate_is_yard", "nameplate_is_dig", "nameplate_is_game", "nameplate_is_ghost", "nameplate_is_green", "nameplate_is_sand", "nameplate_is_trees", "nameplate_Mira_Cafeteria", "nameplate_Mira_Glass", "nameplate_Mira_Tiles", "nameplate_Mira_Vines", "nameplate_Mira_Wood", "nameplate_hw_candy", "nameplate_hw_woods", "nameplate_hw_pumpkin" };
                    //help me out with the nameplates, couldn't find them in the game assets
                    for (auto player : GetAllPlayerControl()) {
                        std::string name = "";
                        if (State.confuserNameGeneration == 0 || (State.confuserNameGeneration == 2 && State.cyclerUserNames.empty()))
                            name = GenerateRandomString();
                        else if (State.confuserNameGeneration == 1)
                            name = GenerateRandomString(true);
                        else if (State.confuserNameGeneration == 2) {
                            if (!State.cyclerUserNames.empty())
                                name = State.cyclerUserNames[randi(0, (int)State.cyclerUserNames.size() - 1)] + "<size=0>" + std::to_string(player->fields.PlayerId) + "</size>";
                        }
                        else
                            name = GenerateRandomString();
                        queue->push(new RpcForceName(player, name));
                        queue->push(new RpcForceColor(player, randi(0, 17)));
                        queue->push(new RpcForceHat(player, convert_to_string(availableHats[randi(0, (int)availableHats.size() - 1)])));
                        queue->push(new RpcForceSkin(player, convert_to_string(availableSkins[randi(0, (int)availableSkins.size() - 1)])));
                        queue->push(new RpcForceVisor(player, convert_to_string(availableVisors[randi(0, (int)availableVisors.size() - 1)])));
                        queue->push(new RpcForcePet(player, convert_to_string(availablePets[randi(0, (int)availablePets.size() - 1)])));
                        queue->push(new RpcForceNamePlate(player, convert_to_string(availableNamePlates[randi(0, (int)availableNamePlates.size() - 1)])));
                    }
                }

                ImGui::Text("Confundir al:");
                if (ToggleButton("Entrar a Sala", &State.confuseOnJoin)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Comenzar Partida", &State.confuseOnStart)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Asesinar", &State.confuseOnKill)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Usar Alcantarilla", &State.confuseOnVent)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Reunion", &State.confuseOnMeeting)) {
                    State.Save();
                }
            }
            if (!State.SafeMode && ImGui::CollapsingHeader("Opciones de Nombres de Confusor")) {
                if (CustomListBoxInt("Generador de Nombres", &State.confuserNameGeneration, NAMEGENERATION, 75 * State.dpiScale)) {
                    State.Save();
                }
                if (State.confuserNameGeneration == 2) {
                    if (State.cyclerUserNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sin nombres en lista; se usara combinacion de palabras.");
                    static std::string newName = "";
                    InputString("Nuevo Nombre ", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::SameLine();
                    if (AnimatedButton("Agregar Nombre ")) {
                        State.cyclerUserNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }
                    if (!(IsHost() || !State.SafeMode) && !IsNameValid(newName)) {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nombre bloqueado por anticheat.");
                    }
                    if (!State.cyclerUserNames.empty()) {
                        static int selectedNameIndex = 0;
                        selectedNameIndex = std::clamp(selectedNameIndex, 0, (int)State.cyclerUserNames.size() - 1);
                        std::vector<const char*> nameVector(State.cyclerUserNames.size(), nullptr);
                        for (size_t i = 0; i < State.cyclerUserNames.size(); i++) {
                            nameVector[i] = State.cyclerUserNames[i].c_str();
                        }
                        CustomListBoxInt("Nombre a Eliminar", &selectedNameIndex, nameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("Eliminar "))
                            State.cyclerUserNames.erase(State.cyclerUserNames.begin() + selectedNameIndex);
                    }
                }
            }
            ImGui::Dummy(ImVec2(4, 2)* State.dpiScale);
            bool isPresetDeleted = false;
            if (ImGui::CollapsingHeader("Perfiles de Cosmeticos", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Dummy(ImVec2(4, 2) * State.dpiScale);
                if (ToggleButton("Auto-aplicar al Entrar", &State.AutoApplyCosmeticPreset))
                    State.Save();
                ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                if (!State.CosmeticPresets.empty()) {
                    std::vector<const char*> names;
                    for (auto& p : State.CosmeticPresets) names.push_back(p.Name.c_str());
                    CustomListBoxInt("Perfil", &State.SelectedCosmeticPreset, names, 200.0f * State.dpiScale, ImVec4(0, 0, 0, 0), 0);
                    ImGui::SameLine();
                    if (AnimatedButton("Aplicar##cosmeticpreset")) {
                        ApplyCosmeticPreset(State.CosmeticPresets[std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1)]);
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Actualizar##cosmeticpreset")) {
                        auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                        if (outfit != nullptr) {
                            int idx = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                            auto& p = State.CosmeticPresets[idx];
                            p.ColorId = outfit->fields.ColorId;
                            p.HatId = outfit->fields.HatId ? convert_from_string(outfit->fields.HatId) : "";
                            p.SkinId = outfit->fields.SkinId ? convert_from_string(outfit->fields.SkinId) : "";
                            p.VisorId = outfit->fields.VisorId ? convert_from_string(outfit->fields.VisorId) : "";
                            p.PetId = outfit->fields.PetId ? convert_from_string(outfit->fields.PetId) : "";
                            p.NamePlateId = outfit->fields.NamePlateId ? convert_from_string(outfit->fields.NamePlateId) : "";
                            State.Save();
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Eliminar##cosmeticpreset")) {
                        int idx = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                        State.CosmeticPresets.erase(State.CosmeticPresets.begin() + idx);
                        if (State.CosmeticPresets.size() != 0)
                            State.SelectedCosmeticPreset = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                        isPresetDeleted = true;
                        State.Save();
                    }
                }
                else {
                    ImGui::TextDisabled("No hay perfiles de cosmeticos guardados.");
                }

                if (!isPresetDeleted) {
                    ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                    static std::string newCosmeticName = "Mi Atuendo";
                    ImGui::SetNextItemWidth(160 * State.dpiScale);
                    InputString("Nombre de Perfil##cosmetic", &newCosmeticName);
                    ImGui::SameLine();
                    if (AnimatedButton("Guardar Actual##cosmeticpreset")) {
                        auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                        if (outfit != nullptr) {
                            Settings::CosmeticPreset p;
                            p.Name = newCosmeticName.empty() ? "Perfil" : newCosmeticName;
                            p.ColorId = outfit->fields.ColorId;
                            p.HatId = outfit->fields.HatId ? convert_from_string(outfit->fields.HatId) : "";
                            p.SkinId = outfit->fields.SkinId ? convert_from_string(outfit->fields.SkinId) : "";
                            p.VisorId = outfit->fields.VisorId ? convert_from_string(outfit->fields.VisorId) : "";
                            p.PetId = outfit->fields.PetId ? convert_from_string(outfit->fields.PetId) : "";
                            p.NamePlateId = outfit->fields.NamePlateId ? convert_from_string(outfit->fields.NamePlateId) : "";
                            State.CosmeticPresets.push_back(p);
                            State.SelectedCosmeticPreset = (int)State.CosmeticPresets.size() - 1;
                            State.Save();
                        }
                    }
                }
            }
        }

        if (openAntiExploit) {
            if (ToggleButton("Sin Penalizacion por Desconexion", &State.AntiExploit_DisconnectPenalties)) State.Save();
            if (ToggleButton("Resistir Sabotajes Dirigidos (No Host)", &State.AntiExploit_UnauthorizedSabotages)) State.Save();
            if (ToggleButton("Resistir Teletransportes No Autorizados", &State.AntiExploit_UnauthorizedTeleports)) State.Save();
            if (ToggleButton("Resistir Tirolinas No Autorizadas", &State.AntiExploit_UnauthorizedZiplines)) State.Save();
            if (ToggleButton("Resistir Intentos de Ban", &State.AntiExploit_AttemptToBan)) State.Save();

            ImGui::NewLine();
            ImGui::Text("Anti-Exploits para Hosts");
            if (ToggleButton("Resistir Votos de Expulsion hacia Ti", &State.AntiExploit_VotekicksAgainstSelfHost)) State.Save();
            if (ToggleButton("Prevenir Intentos de Crashear Sala", &State.AntiExploit_CrashLobbyHost)) State.Save();
        }

        if (openTextEditor) {
            InputString("Entrada", &originalText);
            editedText = GetTextEditorName(originalText);
            InputString("Salida", &editedText);
            ImGui::SameLine();
            if (AnimatedButton("Copiar")) ClipboardHelper_PutClipboardString(convert_to_string(editedText), NULL);

            ToggleButton("Cursiva", &italicName);
            ImGui::SameLine();
            ToggleButton("Subrayado", &underlineName);
            ImGui::SameLine();
            ToggleButton("Tachado", &strikethroughName);
            ImGui::SameLine();
            ToggleButton("Negrita", &boldName);
            ImGui::SameLine();
            ToggleButton("Sin Salto de Linea", &nobrName);

            ImGui::ColorEdit4("Color Degradado Inicial", (float*)&nameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
            ImGui::SameLine();
            ImGui::ColorEdit4("Color Degradado Final", (float*)&nameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
            ImGui::SameLine();
            ToggleButton("Coloreado", &coloredName);

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            ToggleButton("Fuente", &font);
            ImGui::SameLine();
            CustomListBoxInt(" ", &fontType, FONTS, 160.f * State.dpiScale);
            ImGui::Dummy(ImVec2(-5, -5) * State.dpiScale);
            if (State.Font) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Nota: El apodo blanco no sera visible en el chat"));

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            /*if (ToggleButton("Material", &State.Material)) {
                State.Save();
            }
            ImGui::SameLine();
            if (CustomListBoxInt(" Some materials are not supported", &State.MaterialType, MATERIALS, 160.f * State.dpiScale)) {
                State.Save();
            }*/

            ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
            ToggleButton("Tamano", &resizeName);

            ImGui::SameLine();
            ImGui::InputFloat("Tamano del Nombre", &nameSize);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Sangria", &indentName);

            ImGui::SameLine();
            ImGui::InputFloat("Nivel de Sangria", &indentLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Espaciado Caracteres", &cspaceName);

            ImGui::SameLine();
            ImGui::InputFloat("Nivel de Espaciado", &cspaceLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Monoespacio", &mspaceName);

            ImGui::SameLine();
            ImGui::InputFloat("Nivel de Monoespacio", &mspaceLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Desplazamiento V", &voffsetName);

            ImGui::SameLine();
            ImGui::InputFloat("Nivel Desplazamiento V", &voffsetLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Rotar", &rotateName);

            ImGui::SameLine();
            ImGui::InputFloat("Angulo de Rotacion", &rotateAngle);
        }
        ImGui::EndChild();
        ImGui::EndChild();
    }
}
