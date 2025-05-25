#include "raylib.h"
#include "MapData.h"
using namespace std;

vector<EnvElement> blocks = {

	{-200, 600, 3350, 200, VIOLET, false}, // SUELO
	{3250, 600, 760, 200, VIOLET, false}, // SUELO
	{4160, 600, 3063, 200, VIOLET, false}, // SUELO
	{7320, 600, 3000, 200, VIOLET, false}, // SUELO

	{650, 400, 50, 50, RED, false},	//PRIMER ? /MONEDAS

	//Conjuto de bloques
	{850, 400, 50, 50, GREEN, false},
	{900, 400, 50, 50, BROWN, false}, // TIENE UN CHAMPIÑON
	{950, 400, 50, 50, GREEN, false},
	{1000, 400, 50, 50, RED, false}, // MONEDAS
	{1050, 400, 50, 50, GREEN, false},
	//Bloque superior
	{950, 200, 50, 50, RED, false}, //MONEDAS

	//Tuberias
	{1250, 500, 100, 100, PURPLE, false},
	{1675, 450, 100, 150, PURPLE, false},
	{2075, 400, 100, 200, PURPLE, false},
	{2600, 400, 100, 200, PURPLE, false},

	//Bloque oculto (ya se hará) -Vida extra-
	 //de colores queda el LIGHTGRAY

	//Boquete 1
	{3150, 590, 50, 50, BLUE, false},
	{3200, 590, 50, 50, BLUE, false},
	{3150, 640, 50, 50, BLUE, false},
	{3200, 640, 50, 50, BLUE, false},
	{3150, 650, 50, 50, BLUE, false},
	{3200, 650, 50, 50, BLUE, false},
	//Segundo conjuto de bloques
		//Zona inferior
	{3550, 400, 50, 50, GREEN, false},
	{3600, 400, 50, 50, BROWN, false}, //Champiñon
	{3650, 400, 50, 50, GREEN, false},
	//Zona superior
	{3700, 200, 50, 50, GREEN, false},
	{3750, 200, 50, 50, GREEN, false},
	{3800, 200, 50, 50, GREEN, false},
	{3850, 200, 50, 50, GREEN, false},
	{3900, 200, 50, 50, GREEN, false},
	{3950, 200, 50, 50, GREEN, false},
	{4000, 200, 50, 50, GREEN, false},
	{4050, 200, 50, 50, GREEN, false},
	//Boquete 2
	{4010, 590, 50, 50, BLUE, false},
	{4060, 590, 50, 50, BLUE, false},
	{4110, 590, 50, 50, BLUE, false},

	{4010, 640, 50, 50, BLUE, false},
	{4060, 640, 50, 50, BLUE, false},
	{4110, 640, 50, 50, BLUE, false},

	{4010, 650, 50, 50, BLUE, false},
	{4060, 650, 50, 50, BLUE, false},
	{4110, 650, 50, 50, BLUE, false},

	//Zona superior 2
	{4250, 200, 50, 50, GREEN, false},
	{4300, 200, 50, 50, GREEN, false},
	{4350, 200, 50, 50, GREEN, false},
	{4400, 200, 50, 50, RED, false},//Monedas
	//Bloque inferior
	{4400, 400, 50, 50, MAGENTA, false}, //Moneda

	//Bloques extrella
	{4700, 400, 50, 50, GREEN, false},
	{4750, 400, 50, 50, PINK, false}, //Estrella

	//Bloques ?
		//Inferior
	{5000, 400, 50, 50, RED, false}, //Monedas
	{5125, 400, 50, 50, RED, false}, //Monedas
	{5250, 400, 50, 50, RED, false}, //Monedas
	//Superior
	{5125, 200, 50, 50, BROWN, false}, //Champiñon

	//Secuencia de bloques
	{5550, 400, 50, 50, GREEN, false},

	{5700, 200, 50, 50, GREEN, false},
	{5750, 200, 50, 50, GREEN, false},
	{5800, 200, 50, 50, GREEN, false},

	{6000, 200, 50, 50, GREEN, false},
	{6050, 200, 50, 50, GREEN, false},
	{6100, 200, 50, 50, GREEN, false},
	{6150, 200, 50, 50, GREEN, false},

	{6050, 400, 50, 50, GREEN, false},
	{6100, 400, 50, 50, GREEN, false},

	//Primera escalera
	//Primer escalon
	{6310, 550, 50, 50, GRAY, false},
	{6360, 550, 50, 50, GRAY, false},
	{6410, 550, 50, 50, GRAY, false},
	//Segundo escalon
	{6360, 500, 50, 50, GRAY, false},
	{6410, 500, 50, 50, GRAY, false},
	//Tercer escalon
	{6410, 450, 50, 50, GRAY, false},
	//Quarto escalon
	{6460, 400, 50, 200, GRAY, false },

	//-Invertida-//
	//Primer escalón
	{6640, 550, 50, 50, GRAY, false},
	{6690, 550, 50, 50, GRAY, false},
	{6740, 550, 50, 50, GRAY, false},
	//Segundo escalón
	{6640, 500, 50, 50, GRAY, false},
	{6690, 500, 50, 50, GRAY, false},
	//Tercer escalón
	{6640, 450, 50, 50, GRAY, false},
	//Quarto escalón
	{6590, 400, 50,  200, GRAY, false },

	//Segunda escalera
	//Primer escalon
	{6970, 550, 50, 50, GRAY, false},
	{7020, 550, 50, 50, GRAY, false},
	{7070, 550, 50, 50, GRAY, false},
	{7120, 550, 50, 50, GRAY, false},

	//Segundo escalon
	{7020, 500, 50, 50, GRAY, false},
	{7070, 500, 50, 50, GRAY, false},
	{7120, 500, 50, 50, GRAY, false},

	//Tercer escalon
	{7070, 450, 50, 50, GRAY, false},
	{7120, 450, 50, 50, GRAY, false},

	//Quarto escalon
	{7120, 400, 50, 50, GRAY, false},
	{7170, 400, 50, 200, GRAY, false},

	//BOQUETE 3
	{ 7222, 590, 50, 50, BLUE, false },
	{ 7270, 590, 50, 50, BLUE, false },
	{ 7222, 640, 50, 50, BLUE, false },
	{ 7270, 640, 50, 50, BLUE, false },
	{ 7222, 650, 50, 50, BLUE, false },
	{ 7270, 650, 50, 50, BLUE, false },

	//-Invertida-//
	//Primer escalón
	{7370, 550, 50, 50, GRAY, false},
	{7420, 550, 50, 50, GRAY, false},
	{7470, 550, 50, 50, GRAY, false},

	//Segundo escalón
	{7370, 500, 50, 50, GRAY, false},
	{7420, 500, 50, 50, GRAY, false},

	//Tercer escalón
	{7370, 450, 50, 50, GRAY, false},

	//Quarto escalón
	{7320, 400, 50, 200, GRAY, false },

	//Tuberias finales
	{7700, 500, 100, 100, PURPLE, false },
	//Bloques intermedios
	{7950, 400, 50, 50, GREEN, false },
	{8000, 400, 50, 50, GREEN, false },
	{8050, 400, 50, 50, RED, false },
	{8100, 400, 50, 50, GREEN, false },
	{8460, 500, 100, 100, PURPLE, false },

	//ESCALERA FINAL
	// Primer escalón (base de 9 bloques)
	{8570, 550, 50, 50, GRAY, false},
	{8620, 550, 50, 50, GRAY, false},
	{8670, 550, 50, 50, GRAY, false},
	{8720, 550, 50, 50, GRAY, false},
	{8770, 550, 50, 50, GRAY, false},
	{8820, 550, 50, 50, GRAY, false},
	{8870, 550, 50, 50, GRAY, false},
	{8920, 550, 50, 50, GRAY, false},

	// Segundo escalón (8 bloques)
	{8620, 500, 50, 50, GRAY, false},
	{8670, 500, 50, 50, GRAY, false},
	{8720, 500, 50, 50, GRAY, false},
	{8770, 500, 50, 50, GRAY, false},
	{8820, 500, 50, 50, GRAY, false},
	{8870, 500, 50, 50, GRAY, false},
	{8920, 500, 50, 50, GRAY, false},

	// Tercer escalón (7 bloques)
	{8670, 450, 50, 50, GRAY, false},
	{8720, 450, 50, 50, GRAY, false},
	{8770, 450, 50, 50, GRAY, false},
	{8820, 450, 50, 50, GRAY, false},
	{8870, 450, 50, 50, GRAY, false},
	{8920, 450, 50, 50, GRAY, false},

	// Cuarto escalón (6 bloques)
	{8720, 400, 50, 50, GRAY, false},
	{8770, 400, 50, 50, GRAY, false},
	{8820, 400, 50, 50, GRAY, false},
	{8870, 400, 50, 50, GRAY, false},
	{8920, 400, 50, 50, GRAY, false},

	// Quinto escalón (5 bloques)
	{8770, 350, 50, 50, GRAY, false},
	{8820, 350, 50, 50, GRAY, false},
	{8870, 350, 50, 50, GRAY, false},
	{8920, 350, 50, 50, GRAY, false},

	// Sexto escalón (4 bloques)
	{8820, 300, 50, 50, GRAY, false},
	{8870, 300, 50, 50, GRAY, false},
	{8920, 300, 50, 50, GRAY, false},

	// Séptimo escalón (3 bloques)
	{8870, 250, 50, 50, GRAY, false},
	{8920, 250, 50, 50, GRAY, false},

	// Octavo escalón (2 bloques)
	{8920, 200, 50, 50, GRAY, false},
	{8970, 200, 50, 400, GRAY, false},

	//Flag
	{9375, 550, 50, 50, ORANGE, false },

	//Cueba escenario
		//Suelo
	{ -112, -1500, 50, 50, BLACK, false },
	{ -62, -1500, 50, 50, BLACK, false },
	{ -12, -1500, 50, 50, BLACK, false },
	{ 38, -1500, 50, 50, BLACK, false },
	{ 88, -1500, 50, 50, BLACK, false },
	{ 138, -1500, 50, 50, BLACK, false },
	{ 188, -1500, 50, 50, BLACK, false },
	{ 238, -1500, 50, 50, BLACK, false },
	{ 288, -1500, 50, 50, BLACK, false },
	{ 338, -1500, 50, 50, BLACK, false },
	{ 388, -1500, 50, 50, BLACK, false },
	{ 438, -1500, 50, 50, BLACK, false },
	{ 488, -1500, 50, 50, BLACK, false },
	{ 538, -1500, 50, 50, BLACK, false },
	{ 588, -1500, 50, 50, BLACK, false },
	{ 638, -1500, 50, 50, BLACK, false },
	{ 688, -1500, 50, 50, BLACK, false },
	{ 688, -1500, 50, 50, BLACK, false },
	{ 738, -1500, 50, 50, BLACK, false },

	{ -112, -1450, 50, 50, BLACK, false },
	{ -62, -1450, 50, 50, BLACK, false },
	{ -12, -1450, 50, 50, BLACK, false },
	{ 38, -1450, 50, 50, BLACK, false },
	{ 88, -1450, 50, 50, BLACK, false },
	{ 138, -1450, 50, 50, BLACK, false },
	{ 188, -1450, 50, 50, BLACK, false },
	{ 238, -1450, 50, 50, BLACK, false },
	{ 288, -1450, 50, 50, BLACK, false },
	{ 338, -1450, 50, 50, BLACK, false },
	{ 388, -1450, 50, 50, BLACK, false },
	{ 438, -1450, 50, 50, BLACK, false },
	{ 488, -1450, 50, 50, BLACK, false },
	{ 538, -1450, 50, 50, BLACK, false },
	{ 588, -1450, 50, 50, BLACK, false },
	{ 638, -1450, 50, 50, BLACK, false },
	{ 688, -1450, 50, 50, BLACK, false },
	{ 738, -1450, 50, 50, BLACK, false },

	//Paredes-
	
	{ -112, -2000, 50, 350, LIGHTGRAY, false },

	{ 688, -2000, 50, 350, LIGHTGRAY, false },

	//techo
	{ 88, -2000, 50, 50, LIGHTGRAY, false },
	{ 138, -2000, 50, 50, LIGHTGRAY, false },
	{ 188, -2000, 50, 50, LIGHTGRAY, false },
	{ 238, -2000, 50, 50, LIGHTGRAY, false },
	{ 288, -2000, 50, 50, LIGHTGRAY, false },
	{ 338, -2000, 50, 50, LIGHTGRAY, false },
	{ 388, -2000, 50, 50, LIGHTGRAY, false },

	//Zona monedas
	{ 88, -1550, 50, 50, LIGHTGRAY, false },
	{ 138, -1550, 50, 50, LIGHTGRAY, false },
	{ 188, -1550, 50, 50, LIGHTGRAY, false },
	{ 238, -1550, 50, 50, LIGHTGRAY, false },
	{ 288, -1550, 50, 50, LIGHTGRAY, false },
	{ 338, -1550, 50, 50, LIGHTGRAY, false },
	{ 388, -1550, 50, 50, LIGHTGRAY, false },

	{ 88, -1600, 50, 50, LIGHTGRAY, false },
	{ 138, -1600, 50, 50, LIGHTGRAY, false },
	{ 188, -1600, 50, 50, LIGHTGRAY, false },
	{ 238, -1600, 50, 50, LIGHTGRAY, false },
	{ 288, -1600, 50, 50, LIGHTGRAY, false },
	{ 338, -1600, 50, 50, LIGHTGRAY, false },
	{ 388, -1600, 50, 50, LIGHTGRAY, false },

	{ 88, -1650, 50, 50, LIGHTGRAY, false },
	{ 138, -1650, 50, 50, LIGHTGRAY, false },
	{ 188, -1650, 50, 50, LIGHTGRAY, false },
	{ 238, -1650, 50, 50, LIGHTGRAY, false },
	{ 288, -1650, 50, 50, LIGHTGRAY, false },
	{ 338, -1650, 50, 50, LIGHTGRAY, false },
	{ 388, -1650, 50, 50, LIGHTGRAY, false },

	//Monedas
	{ 96, -1700, 30, 50, YELLOW, false },
	{ 146, -1700, 30, 50, YELLOW, false },
	{ 196, -1700, 30, 50, YELLOW, false },
	{ 246, -1700, 30, 50, YELLOW, false },
	{ 296, -1700, 30, 50, YELLOW, false },
	{ 346, -1700, 30, 50, YELLOW, false },
	{ 396, -1700, 30, 50, YELLOW, false },

	{ 96, -1790, 30, 50, YELLOW, false },
	{ 146, -1790, 30, 50, YELLOW, false },
	{ 196, -1790, 30, 50, YELLOW, false },
	{ 246, -1790, 30, 50, YELLOW, false },
	{ 296, -1790, 30, 50, YELLOW, false },
	{ 346, -1790, 30, 50, YELLOW, false },
	{ 396, -1790, 30, 50, YELLOW, false },

	{ 146, -1880, 30, 50, YELLOW, false },
	{ 196, -1880, 30, 50, YELLOW, false },
	{ 246, -1880, 30, 50, YELLOW, false },
	{ 296, -1880, 30, 50, YELLOW, false },
	{ 346, -1880, 30, 50, YELLOW, false },

	//tuberia
	{ 578, -1600, 100, 100, PURPLE, false },
};