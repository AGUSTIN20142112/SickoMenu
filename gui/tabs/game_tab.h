#pragma once
#include <vector>
#include "state.hpp"
#include "utility.h"

namespace GameTab {
	const std::vector<const char*> KILL_DISTANCE = { "Corta", "Media", "Larga", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
	const std::vector<const char*> TASKBARUPDATES = { "Siempre", "Reuniones", "Nunca", "3", "4", "5", "6" };
	const std::vector<const char*> SMAC_PUNISHMENTS = { "No hacer nada", "Advertirse a uno mismo"/*, "Warn All (Chat)", State.SafeMode ? "Attempt to Ban" : "Attempt to Kick"*/};
	const std::vector<const char*> SMAC_HOST_PUNISHMENTS = { "No hacer nada", "Advertirse a uno mismo"/*, "Warn All (Chat)"*/, "Expulsar", "Banear"};
	const std::vector<const char*> SHIPVENTS = { "Admin", "Pasillo", "Cafetería", "Electricidad", "Motor Superior", "Seguridad", "Ala Médica", "Armería", "Reactor Inferior", "Motor Inferior", "Escudos", "Reactor Superior", "Navegación Superior", "Navegación Inferior" };
	const std::vector<const char*> HQVENTS = { "Balcón", "Cafetería", "Reactor", "Laboratorio", "Oficina", "Admin", "Invernadero", "Ala Médica", "Descontaminación", "Vestidores", "Plataforma de Lanzamiento" };
	const std::vector<const char*> PBVENTS = { "Seguridad", "Electricidad", "O2", "Comunicaciones", "Oficina", "Admin", "Laboratorio", "Piscina de Lava", "Almacén", "Sísmico Derecho", "Sísmico Izquierdo", "Fuera de Admin" };
	const std::vector<const char*> AIRSHIPVENTS = { "Bóveda", "Cabina", "Mirador", "Motor", "Cocina", "Sala Principal Inferior", "Sala Principal Superior", "Habitación de Brecha Derecha", "Habitación de Brecha Izquierda", "Duchas", "Archivos", "Bahía de Carga" };
	const std::vector<const char*> FUNGLEVENTS = { "Comunicaciones", "Cocina", "Mirador", "Fuera de Dormitorios", "Laboratorio", "Reactor", "Jungla (Laboratorio)", "Jungla (Invernadero)", "Zona de Salpicaduras", "Cafetería" };
	const std::vector<std::string> PLATFORM_FILTERS = { "Epic Games (PC)", "Steam (PC)", "Mac", "Microsoft Store (PC)", "itch.io (PC)", "iOS/iPadOS (Movil)", "Android (Movil)", "Nintendo Switch (Consola)", "Xbox (Consola)", "PlayStation (Consola)", "Desconocido" };
	const std::vector<const char*> COLORS = { "Rojo", "Azul", "Verde", "Rosa", "Naranja", "Amarillo", "Negro", "Blanco", "Morado", "Marrón", "Cian", "Lima", "Granate", "Rosa Claro", "Plátano", "Gris", "Canela", "Coral" };
	const std::vector<const char*> HOSTCOLORS = { "Rojo", "Azul", "Verde", "Rosa", "Naranja", "Amarillo", "Negro", "Blanco", "Morado", "Marrón", "Cian", "Lima", "Granate", "Rosa Claro", "Plátano", "Gris", "Canela", "Coral", "Verde Fuerte" };
	const ColorMapping COLOR_NAMES_COLOR[] = {
		{"Rojo",			ImColor::ImColor(0xFF1111C6)}, // 0xAABBGGRR
		{"Azul",			ImColor::ImColor(0xFFD22E13)},
		{"Verde",			ImColor::ImColor(0xFF2D8011)},
		{"Rosa",			ImColor::ImColor(0xFFBB54EE)},
		{"Naranja",			ImColor::ImColor(0xFF0D7DF0)},
		{"Amarillo",		ImColor::ImColor(0xFF57F6F6)},
		{"Negro",			ImColor::ImColor(0xFF4E473F)},
		{"Blanco",			ImColor::ImColor(0xFFF1E1D7)},
		{"Morado",			ImColor::ImColor(0xFFBC2F6B)},
		{"Marrón",			ImColor::ImColor(0xFF1E4971)},
		{"Cian",			ImColor::ImColor(0xFFDDFF38)},
		{"Lima",			ImColor::ImColor(0xFF39F050)},
		{"Granate",			ImColor::ImColor(0xFF2E1D5F)},
		{"Rosa Claro",		ImColor::ImColor(0xFFD3C0EC)},
		{"Plátano",			ImColor::ImColor(0xFFA8E7F0)},
		{"Gris",			ImColor::ImColor(0xFF938575)},
		{"Canela",			ImColor::ImColor(0xFF778891)},
		{"Coral",			ImColor::ImColor(0xFF6464D7)},
		{"Verde Fuerte",    ImColor::ImColor(0xFF62A626)},
	};
	void Render();
	void OpenSubGroup(const std::string& name);
}
