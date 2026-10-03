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
#include <cstdlib>

namespace AboutTab {
    enum Groups {
        Welcome,
        Credits
    };

    static bool openWelcome = true; //default to welcome tab group
    static bool openCredits = false;

    void CloseOtherGroups(Groups group) {
        openWelcome = group == Groups::Welcome;
        openCredits = group == Groups::Credits;
    }

    const ImVec4 SickoCol = ImVec4(1.f, 0.f, 0.424f, 1.f);
    const ImVec4 RedCol = ImVec4(1.f, 0.f, 0.f, 1.0f);
    const ImVec4 AumCol = ImVec4(1.f, 0.3333f, 0.3333f, 1.0f);
    const ImVec4 GoldCol = ImVec4(1.f, 0.7333f, 0.f, 1.0f);
    const ImVec4 GoatCol = ImVec4(0.937f, 0.004f, 0.263f, 1.0f);
    const ImVec4 DevCol = ImVec4(0.102f, 0.7373f, 0.6118f, 1.0f);
    const ImVec4 ContributorCol = ImVec4(0.3804f, 0.4314f, 0.7961f, 1.0f);
    const ImVec4 PreReleaseCol = ImVec4(0.656f, 0.f, 1.f, 1.f);

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###AboutButtons", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Bienvenido", openWelcome)) {
            CloseOtherGroups(Groups::Welcome);
        }
        ImGui::SameLine();
        if (TabGroup("Creditos", openCredits)) {
            CloseOtherGroups(Groups::Credits);
        }

        ImGui::BeginChild("###About", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (openWelcome) {
            ImGui::Text(std::format("¡Bienvenido{} a ", State.HasOpenedMenuBefore ? " de nuevo" : "").c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            if (State.SickoVersion.find("pr") != std::string::npos || State.SickoVersion.find("rc") != std::string::npos)
                ImGui::TextColored(PreReleaseCol, std::format(" {}", State.SickoVersion).c_str());
            else
                ImGui::TextColored(GoldCol, std::format(" {}", State.SickoVersion).c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" por ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoatCol, "g0aty");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" es una potente utilidad para Among Us.");
            ImGui::Text("¡Busca mejorar la experiencia de juego para todos!");
            ImGui::Text("¡Usa el boton \"Buscar Actualizaciones\" para descargar la version mas reciente!");
            if (ColoredButton(DevCol, "GitHub")) {
                OpenLink("https://github.com/g0aty/SickoMenu");
            }
            ImGui::SameLine();
            if (ColoredButton(GoldCol, "Buscar Actualizaciones")) {
                OpenLink("https://github.com/g0aty/SickoMenu/releases/latest");
            }
            ImGui::SameLine();
            if (ColoredButton(State.RgbColor, "Donar")) {
                OpenLink("https://ko-fi.com/g0aty");
            }
            ImGui::Text("¡Unete al servidor de Discord para soporte, reportes y novedades!");
            if (ColoredButton(ContributorCol, "¡Unirse a Discord!")) {
                OpenLink("https://dsc.gg/sickos"); //SickoMenu discord invite
            }

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" es software libre y de codigo abierto.");

            if (State.SickoVersion.find("pr") != std::string::npos || State.SickoVersion.find("rc") != std::string::npos) {
                if (State.SickoVersion.find("pr") != std::string::npos) ImGui::TextColored(State.RgbColor, "Tienes acceso a versiones previas, ¡disfrutalo!");
                else ImGui::TextColored(State.RgbColor, "Tienes acceso a la release candidate, ¡disfrutalo!");
                BoldText("Si no tienes acceso al canal de pre-releases en Discord y no compilaste tu mismo,", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("por favor reportalo creando un ticket en nuestro servidor de Discord.", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            else {
                ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Si pagaste por este menu, exige un reembolso de inmediato.");
                BoldText("Asegurate de haber descargado la ultima version de SickoMenu desde GitHub o", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("nuestro Discord oficial!", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            //hopefully stop people from reselling a foss menu for actual money

            if (State.AprilFoolsMode) {
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                auto DiddyCol = ImVec4(0.79f, 0.03f, 1.f, 1.f);
                /*ImGui::TextColored(DiddyCol, std::format("You now have access to a brand new mode: {} Mode!", State.DiddyPartyMode ? "Diddy Party" :
                (IsChatCensored() || IsStreamerMode() ? "F***son" : "Fuckson")).c_str());
                ImGui::TextColored(DiddyCol, "Find all the new features and enjoy!");
                if (ToggleButton("Diddy Party Mode", &State.DiddyPartyMode)) {
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }*/
                ImGui::TextColored(DiddyCol, "Happy April Fools'!");
                ImGui::TextColored(DiddyCol, "This is NOT a real update as the official release is not yet ready.");
                ImGui::TextColored(DiddyCol, "Please wait for the official release to support the latest versions of Among Us!");
            }
        }

        if (openCredits) {
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" is a fork of");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(RedCol, " (archived)");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(", go check it out!");

            if (ColoredButton(AumCol, "AmongUsMenu")) {
                OpenLink("https://github.com/BitCrackers/AmongUsMenu");
            }
            BoldText("Lead Developer", GoldCol);
            if (ColoredButton(GoatCol, "g0aty")) {
                OpenLink("https://github.com/g0aty");
            }

            BoldText("Developers", DevCol);
            if (ColoredButton(DevCol, "GDjkhp")) {
                OpenLink("https://github.com/GDjkhp");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "Reycko")) {
                OpenLink("https://github.com/Reycko");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "astra1dev")) {
                OpenLink("https://github.com/astra1dev");
            }

            if (ColoredButton(DevCol, "Luckyheat")) {
                OpenLink("https://github.com/L9-ILVrnl");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "UN83991956")) {
                OpenLink("https://github.com/UN83991956");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "HarithGamerPk")) {
                OpenLink("https://github.com/HarithGamerPk");
            }

            if (ColoredButton(DevCol, "Hisoka")) {
                OpenLink("https://github.com/mad-magician7");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "WhoAboutYT")) {
                OpenLink("https://github.com/WhoAboutYT");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "M4-sicko")) {
                OpenLink("https://github.com/M4-sicko");
            }

            BoldText("Contributors", ContributorCol);
            if (ColoredButton(ContributorCol, "acer51-doctom")) {
                OpenLink("https://github.com/acer51-doctom");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(ContributorCol, "ZamTDS")) {
                OpenLink("https://github.com/ZamTDS");
            }

            BoldText("Some people who contributed to AUM", AumCol);
            if (ColoredButton(AumCol, "KulaGGin")) {
                OpenLink("https://github.com/KulaGGin");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped with some ImGui code for replay system)");

            if (ColoredButton(AumCol, "tomsa000")) {
                OpenLink("https://github.com/tomsa000");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped with fixing memory leaks and smart pointers)");

            if (ColoredButton(AumCol, "cddjr")) {
                OpenLink("https://github.com/cddjr");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped in updating to the Fungle release)");

            ImGui::Text("Thanks to");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " v0idp");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" for originally creating");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");
            if (ColoredButton(AumCol, "v0idp")) {
                OpenLink("https://github.com/v0idp");
            }

            ImGui::Text("Everyone else who contributed to");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AUM");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" and I couldn't list here.");

            ImGui::Text("Thank you for making ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" possible!");
        }
        ImGui::EndChild();
        ImGui::EndChild();
    }
}