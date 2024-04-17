#pragma once
#include "Binary_Save_Load_Main.h"
struct PBSL {
	unsigned int Sbar;
	unsigned int Sbeat;
	unsigned int Ssep;
	unsigned int Ebar;
	unsigned int Ebeat;
	unsigned int Esep;
	char type;//S, L, D, B
	double idx_or_deg;
	bool is_Strong;
};