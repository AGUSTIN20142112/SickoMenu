#pragma once

namespace SelfTab {
	const std::vector<const char*> TPOPTIONS = { "Ninguno", "Radar", "Cualquier Lugar" };
	const std::vector<const char*> FAKEROLES = { "Tripulante", "Impostor", "Cientifico", "Ingeniero", "Angel Guardian", "Metamorfo", "Fantasma Tripulante", "Fantasma Impostor", "Bocina", "Fantasma", "Rastreador", "Detective", "Vibora", "Juez", "Influencer" };
	const std::vector<const char*> NAMEGENERATION = { "Combinacion de Palabras", "Cadena Aleatoria", "Nombres de Ciclo" };
	const std::vector<const char*> BODYTYPES = { "Normal", "Caballo", "Largo" };
	const std::vector<const char*> FONTS = { "Barlow-Italic", "Barlow-Medium", "Barlow-Bold", "Barlow-SemiBold", "Barlow-SemiBold (Masked)", "Barlow-ExtraBold", "Barlow-BoldItalic", "Barlow-BoldItalic (Masked)", "Barlow-Black", "Barlow-Light", "Barlow-Regular", "Barlow-Regular (Masked)", "Barlow-Regular (Outline)", "Brook", "LiberationSans", "NotoSans", "VCR", "CONSOLA", "digital-7", "OCRAEXT", "DIN_Pro_Bold_700" };
	//const std::vector<const char*> MATERIALS = { "Barlow-Italic Outline", "Barlow-BoldItalic Outline", "Barlow-SemiBold Outline" };
	void Render();
	void OpenSubGroup(const std::string& name);
}
