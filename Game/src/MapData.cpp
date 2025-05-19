#include "raylib.h"
#include "MapData.h"
using namespace std;

vector<EnvElement> blocks = {

	{-200, 600, 3350, 200, GREEN}, // SUELO
	{3250, 600, 760, 200, GREEN}, // SUELO
	{4160, 600, 3063, 200, GREEN}, // SUELO
	{7320, 600, 3000, 200, GREEN}, // SUELO

	{650, 400, 50, 50, GREEN},	//PRIMER ? /MONEDAS

	//Conjuto de bloques
	{850, 400, 50, 50, GREEN},
	{900, 400, 50, 50, GREEN}, // TIENE UN CHAMPIÑON
	{950, 400, 50, 50, GREEN},
	{1000, 400, 50, 50, GREEN}, // MONEDAS
	{1050, 400, 50, 50, GREEN},
	//Bloque superior
	{950, 200, 50, 50, GREEN}, //MONEDAS

	//Tuberias
	{1250, 500, 100, 100, GREEN},
	{1675, 450, 100, 150, GREEN},
	{2075, 400, 100, 200, GREEN},
	{2600, 400, 100, 200, GREEN},

	//Bloque oculto (ya se hará) -Vida extra-
	//Boquete 1
	{3150, 600, 50, 50, BLUE},
	{3200, 600, 50, 50, BLUE},
	{3150, 650, 50, 50, BLUE},
	{3200, 650, 50, 50, BLUE},
	//Segundo conjuto de bloques
		//Zona inferior
	{3550, 400, 50, 50, GREEN},
	{3600, 400, 50, 50, GREEN}, //Champiñon
	{3650, 400, 50, 50, GREEN},
	//Zona superior
	{3700, 200, 50, 50, GREEN},
	{3750, 200, 50, 50, GREEN},
	{3800, 200, 50, 50, GREEN},
	{3850, 200, 50, 50, GREEN},
	{3900, 200, 50, 50, GREEN},
	{3950, 200, 50, 50, GREEN},
	{4000, 200, 50, 50, GREEN},
	{4050, 200, 50, 50, GREEN},
	//Boquete 2
	{4010, 600, 50, 50, BLUE},
	{4060, 600, 50, 50, BLUE},
	{4110, 600, 50, 50, BLUE},

	{4010, 650, 50, 50, BLUE},
	{4060, 650, 50, 50, BLUE},
	{4110, 650, 50, 50, BLUE},

	{4010, 700, 50, 50, BLUE},
	{4060, 700, 50, 50, BLUE},
	{4110, 700, 50, 50, BLUE},

	//Zona superior 2
	{4250, 200, 50, 50, GREEN},
	{4300, 200, 50, 50, GREEN},
	{4350, 200, 50, 50, GREEN},
	{4400, 200, 50, 50, GREEN},//Monedas
	//Bloque inferior
	{4400, 400, 50, 50, GREEN}, //Moneda

	//Bloques extrella
	{4700, 400, 50, 50, GREEN},
	{4750, 400, 50, 50, GREEN}, //Estrella

	//Bloques ?
		//Inferior
	{5000, 400, 50, 50, GREEN}, //Monedas
	{5125, 400, 50, 50, GREEN}, //Monedas
	{5250, 400, 50, 50, GREEN}, //Monedas
	//Superior
	{5125, 200, 50, 50, GREEN}, //Champiñon

	//Secuencia de bloques
	{5550, 400, 50, 50, GREEN},

	{5700, 200, 50, 50, GREEN},
	{5750, 200, 50, 50, GREEN},
	{5800, 200, 50, 50, GREEN},

	{6000, 200, 50, 50, GREEN},
	{6050, 200, 50, 50, GREEN},
	{6100, 200, 50, 50, GREEN},
	{6150, 200, 50, 50, GREEN},

	{6050, 400, 50, 50, GREEN},
	{6100, 400, 50, 50, GREEN},

	//Primera escalera
	//Primer escalon
	{6310, 550, 50, 50, GREEN},
	{6360, 550, 50, 50, GREEN},
	{6410, 550, 50, 50, GREEN},
	{6460, 550, 50, 50, GREEN},
	//Segundo escalon
	{6360, 500, 50, 50, GREEN},
	{6410, 500, 50, 50, GREEN},
	{6460, 500, 50, 50, GREEN},
	//Tercer escalon
	{6410, 450, 50, 50, GREEN},
	{6460, 450, 50, 50, GREEN},
	//Quarto escalon
	{6460, 400, 50, 50, GREEN},
	//-Invertida-//
	//Primer escalón
	{6590, 550, 50, 50, GREEN},
	{6640, 550, 50, 50, GREEN},
	{6690, 550, 50, 50, GREEN},
	{6740, 550, 50, 50, GREEN},
	//Segundo escalón
	{6590, 500, 50, 50, GREEN},
	{6640, 500, 50, 50, GREEN},
	{6690, 500, 50, 50, GREEN},
	//Tercer escalón
	{6590, 450, 50, 50, GREEN},
	{6640, 450, 50, 50, GREEN},
	//Quarto escalón
	{6590, 400, 50, 50, GREEN},

	//Segunda escalera
	//Primer escalon
	{6970, 550, 50, 50, GREEN},
	{7020, 550, 50, 50, GREEN},
	{7070, 550, 50, 50, GREEN},
	{7120, 550, 50, 50, GREEN},
	{7170, 550, 50, 50, GREEN},

	//Segundo escalon
	{7020, 500, 50, 50, GREEN},
	{7070, 500, 50, 50, GREEN},
	{7120, 500, 50, 50, GREEN},
	{7170, 500, 50, 50, GREEN},

	//Tercer escalon
	{7070, 450, 50, 50, GREEN},
	{7120, 450, 50, 50, GREEN},
	{7170, 450, 50, 50, GREEN},

	//Quarto escalon
	{7120, 400, 50, 50, GREEN},
	{7170, 400, 50, 50, GREEN},

	//BOQUETE 3

	//-Invertida-//
	//Primer escalón
	{7320, 550, 50, 50, GREEN},
	{7370, 550, 50, 50, GREEN},
	{7420, 550, 50, 50, GREEN},

	//Segundo escalón
	{7320, 500, 50, 50, GREEN},
	{7370, 500, 50, 50, GREEN},
	{7420, 500, 50, 50, GREEN},

	//Tercer escalón
	{7320, 450, 50, 50, GREEN},
	{7370, 450, 50, 50, GREEN},

	//Quarto escalón
	{7320, 400, 50, 50, GREEN},

	//Tuberias finales
	{7700, 500, 100, 100, GREEN},
	//Bloques intermedios
	{7950, 400, 50, 50, GREEN},
	{8000, 400, 50, 50, GREEN},
	{8050, 400, 50, 50, GREEN},
	{8100, 400, 50, 50, GREEN},
	{8460, 500, 100, 100, GREEN},

	//ESCALERA FINAL
	// Primer escalón (base de 9 bloques)
	{8570, 550, 50, 50, GREEN},
	{8620, 550, 50, 50, GREEN},
	{8670, 550, 50, 50, GREEN},
	{8720, 550, 50, 50, GREEN},
	{8770, 550, 50, 50, GREEN},
	{8820, 550, 50, 50, GREEN},
	{8870, 550, 50, 50, GREEN},
	{8920, 550, 50, 50, GREEN},
	{8970, 550, 50, 50, GREEN},

	// Segundo escalón (8 bloques)
	{8620, 500, 50, 50, GREEN},
	{8670, 500, 50, 50, GREEN},
	{8720, 500, 50, 50, GREEN},
	{8770, 500, 50, 50, GREEN},
	{8820, 500, 50, 50, GREEN},
	{8870, 500, 50, 50, GREEN},
	{8920, 500, 50, 50, GREEN},
	{8970, 500, 50, 50, GREEN},

	// Tercer escalón (7 bloques)
	{8670, 450, 50, 50, GREEN},
	{8720, 450, 50, 50, GREEN},
	{8770, 450, 50, 50, GREEN},
	{8820, 450, 50, 50, GREEN},
	{8870, 450, 50, 50, GREEN},
	{8920, 450, 50, 50, GREEN},
	{8970, 450, 50, 50, GREEN},

	// Cuarto escalón (6 bloques)
	{8720, 400, 50, 50, GREEN},
	{8770, 400, 50, 50, GREEN},
	{8820, 400, 50, 50, GREEN},
	{8870, 400, 50, 50, GREEN},
	{8920, 400, 50, 50, GREEN},
	{8970, 400, 50, 50, GREEN},

	// Quinto escalón (5 bloques)
	{8770, 350, 50, 50, GREEN},
	{8820, 350, 50, 50, GREEN},
	{8870, 350, 50, 50, GREEN},
	{8920, 350, 50, 50, GREEN},
	{8970, 350, 50, 50, GREEN},

	// Sexto escalón (4 bloques)
	{8820, 300, 50, 50, GREEN},
	{8870, 300, 50, 50, GREEN},
	{8920, 300, 50, 50, GREEN},
	{8970, 300, 50, 50, GREEN},

	// Séptimo escalón (3 bloques)
	{8870, 250, 50, 50, GREEN},
	{8920, 250, 50, 50, GREEN},
	{8970, 250, 50, 50, GREEN},

	// Octavo escalón (2 bloques)
	{8920, 200, 50, 50, GREEN},
	{8970, 200, 50, 50, GREEN},

	//Flag
	{9375, 550, 50, 50, GREEN},

	//Cueba escenario
		//Suelo
	{ -112, -500, 850, 100, GREEN },
	//Paredes-
	{ -112, -1000, 50, 200, GREEN },
	{ 688, -1000, 50, 200, GREEN },

	//techo
	{ 88, -1000, 350, 50, GREEN },//1

	//Zona monedas
	{ 88, -650, 350, 150, GREEN },//1
	//tuberia
	{ 578, -600, 100, 100, GREEN },//1
	};