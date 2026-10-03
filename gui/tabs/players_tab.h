#pragma once
#include "utility.h"

namespace PlayersTab {
	const std::vector<const char*> FAKEROLES = { "Tripulante", "Impostor", "Cientifico", "Ingeniero", "Angel Guardian", "Metamorfo", "Fantasma Tripulante", "Fantasma Impostor", "Bocina", "Fantasma", "Rastreador", "Detective", "Vibora", "Juez", "Influencer" };
	const std::vector<const char*> GHOSTROLES = { "Angel Guardian", "Fantasma Tripulante", "Fantasma Impostor", "Influencer" };
	const std::vector<const char*> SHIPVENTS = { "Admin", "Pasillo", "Cafetería", "Electricidad", "Motor Superior", "Seguridad", "Ala Médica", "Armería", "Reactor Inferior", "Motor Inferior", "Escudos", "Reactor Superior", "Navegación Superior", "Navegación Inferior" };
	const std::vector<const char*> HQVENTS = { "Balcón", "Cafetería", "Reactor", "Laboratorio", "Oficina", "Admin", "Invernadero", "Ala Médica", "Descontaminación", "Vestidores", "Plataforma de Lanzamiento" };
	const std::vector<const char*> PBVENTS = { "Seguridad", "Electricidad", "O2", "Comunicaciones", "Oficina", "Admin", "Laboratorio", "Piscina de Lava", "Almacén", "Sísmico Derecho", "Sísmico Izquierdo", "Fuera de Admin" };
	const std::vector<const char*> AIRSHIPVENTS = { "Bóveda", "Cabina", "Mirador", "Motor", "Cocina", "Sala Principal Inferior", "Sala Principal Superior", "Habitación de Brecha Derecha", "Habitación de Brecha Izquierda", "Duchas", "Archivos", "Bahía de Carga" };
	const std::vector<const char*> FUNGLEVENTS = { "Comunicaciones", "Cocina", "Mirador", "Fuera de Dormitorios", "Laboratorio", "Reactor", "Jungla (Laboratorio)", "Jungla (Invernadero)", "Zona de Salpicaduras", "Cafetería" };
	const std::vector<const char*> COLORS = { "Rojo", "Azul", "Verde", "Rosa", "Naranja", "Amarillo", "Negro", "Blanco", "Morado", "Marrón", "Cian", "Lima", "Granate", "Rosa Claro", "Plátano", "Gris", "Canela", "Coral", "Verde Fuerte" };
	const ColorMapping COLOR_NAMES_COLOR[] = {
		{"Rojo",			    ImColor::ImColor(0xFF1111C6)}, // 0xAABBGGRR
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
		{"Verde Fuerte",      ImColor::ImColor(0xFF62A626)},
	};
	void Render();
	void OpenSubGroup(const std::string& name);
}