#include "raylib.h"
#include "MapData.h"
using namespace std;

vector<EnvElement> blocks = {

	{-200, 600, 3350, 200, GREEN, false}, // SUELO
	{3250, 600, 760, 200, GREEN, false}, // SUELO
	{4160, 600, 3063, 200, GREEN, false}, // SUELO
	{7320, 600, 3000, 200, GREEN, false}, // SUELO

	{650, 400, 50, 50, RED, false},	//PRIMER ? /MONEDAS

	//Conjuto de bloques
	{850, 400, 50, 50, GREEN, false},
	{900, 400, 50, 50, BROWN, false}, // TIENE UN CHAMPI�ON
	{950, 400, 50, 50, GREEN, false},
	{1000, 400, 50, 50, RED, false}, // MONEDAS
	{1050, 400, 50, 50, GREEN, false},
	//Bloque superior
	{950, 200, 50, 50, RED, false}, //MONEDAS

	//Tuberias
	{1250, 500, 100, 100, GREEN, false},
	{1675, 450, 100, 150, GREEN, false},
	{2075, 400, 100, 200, GREEN, false},
	{2600, 400, 100, 200, GREEN, false},

	//Bloque oculto (ya se har�) -Vida extra-
	//Boquete 1
	{3150, 600, 50, 50, BLUE, false},
	{3200, 600, 50, 50, BLUE, false},
	{3150, 650, 50, 50, BLUE, false},
	{3200, 650, 50, 50, BLUE, false},
	//Segundo conjuto de bloques
		//Zona inferior
	{3550, 400, 50, 50, GREEN, false},
	{3600, 400, 50, 50, BROWN, false}, //Champi�on
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
	{4010, 600, 50, 50, BLUE, false},
	{4060, 600, 50, 50, BLUE, false},
	{4110, 600, 50, 50, BLUE, false},

	{4010, 650, 50, 50, BLUE, false},
	{4060, 650, 50, 50, BLUE, false},
	{4110, 650, 50, 50, BLUE, false},

	{4010, 700, 50, 50, BLUE, false},
	{4060, 700, 50, 50, BLUE, false},
	{4110, 700, 50, 50, BLUE, false},

	//Zona superior 2
	{4250, 200, 50, 50, GREEN, false},
	{4300, 200, 50, 50, GREEN, false},
	{4350, 200, 50, 50, GREEN, false},
	{4400, 200, 50, 50, RED, false},//Monedas
	//Bloque inferior
	{4400, 400, 50, 50, RED, false}, //Moneda

	//Bloques extrella
	{4700, 400, 50, 50, GREEN, false},
	{4750, 400, 50, 50, PINK, false}, //Estrella

	//Bloques ?
		//Inferior
	{5000, 400, 50, 50, RED, false}, //Monedas
	{5125, 400, 50, 50, RED, false}, //Monedas
	{5250, 400, 50, 50, RED, false}, //Monedas
	//Superior
	{5125, 200, 50, 50, BROWN, false}, //Champi�on

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
	{6310, 550, 50, 50, GREEN, false},
	{6360, 550, 50, 50, GREEN, false},
	{6410, 550, 50, 50, GREEN, false},
	{6460, 550, 50, 50, GREEN, false},
	//Segundo escalon
	{6360, 500, 50, 50, GREEN, false},
	{6410, 500, 50, 50, GREEN, false},
	{6460, 500, 50, 50, GREEN, false},
	//Tercer escalon
	{6410, 450, 50, 50, GREEN, false},
	{6460, 450, 50, 50, GREEN, false},
	//Quarto escalon
	{6460, 400, 50, 50, GREEN, false },
	//-Invertida-//
	//Primer escal�n
	{6590, 550, 50, 50, GREEN, false},
	{6640, 550, 50, 50, GREEN, false},
	{6690, 550, 50, 50, GREEN, false},
	{6740, 550, 50, 50, GREEN, false},
	//Segundo escal�n
	{6590, 500, 50, 50, GREEN, false},
	{6640, 500, 50, 50, GREEN, false},
	{6690, 500, 50, 50, GREEN, false},
	//Tercer escal�n
	{6590, 450, 50, 50, GREEN, false},
	{6640, 450, 50, 50, GREEN, false},
	//Quarto escal�n
	{6590, 400, 50, 50, GREEN, false },

	//Segunda escalera
	//Primer escalon
	{6970, 550, 50, 50, GREEN, false},
	{7020, 550, 50, 50, GREEN, false},
	{7070, 550, 50, 50, GREEN, false},
	{7120, 550, 50, 50, GREEN, false},
	{7170, 550, 50, 50, GREEN, false},

	//Segundo escalon
	{7020, 500, 50, 50, GREEN, false},
	{7070, 500, 50, 50, GREEN, false},
	{7120, 500, 50, 50, GREEN, false},
	{7170, 500, 50, 50, GREEN, false},

	//Tercer escalon
	{7070, 450, 50, 50, GREEN, false},
	{7120, 450, 50, 50, GREEN, false},
	{7170, 450, 50, 50, GREEN, false},

	//Quarto escalon
	{7120, 400, 50, 50, GREEN, false},
	{7170, 400, 50, 50, GREEN, false},

	//BOQUETE 3

	//-Invertida-//
	//Primer escal�n
	{7320, 550, 50, 50, GREEN, false},
	{7370, 550, 50, 50, GREEN, false},
	{7420, 550, 50, 50, GREEN, false},

	//Segundo escal�n
	{7320, 500, 50, 50, GREEN, false},
	{7370, 500, 50, 50, GREEN, false},
	{7420, 500, 50, 50, GREEN, false},

	//Tercer escal�n
	{7320, 450, 50, 50, GREEN, false},
	{7370, 450, 50, 50, GREEN, false},

	//Quarto escal�n
	{7320, 400, 50, 50, GREEN, false },

	//Tuberias finales
	{7700, 500, 100, 100, GREEN, false },
	//Bloques intermedios
	{7950, 400, 50, 50, GREEN, false },
	{8000, 400, 50, 50, GREEN, false },
	{8050, 400, 50, 50, RED, false },
	{8100, 400, 50, 50, GREEN, false },
	{8460, 500, 100, 100, GREEN, false },

	//ESCALERA FINAL
	// Primer escal�n (base de 9 bloques)
	{8570, 550, 50, 50, GREEN, false},
	{8620, 550, 50, 50, GREEN, false},
	{8670, 550, 50, 50, GREEN, false},
	{8720, 550, 50, 50, GREEN, false},
	{8770, 550, 50, 50, GREEN, false},
	{8820, 550, 50, 50, GREEN, false},
	{8870, 550, 50, 50, GREEN, false},
	{8920, 550, 50, 50, GREEN, false},
	{8970, 550, 50, 50, GREEN, false},

	// Segundo escal�n (8 bloques)
	{8620, 500, 50, 50, GREEN, false},
	{8670, 500, 50, 50, GREEN, false},
	{8720, 500, 50, 50, GREEN, false},
	{8770, 500, 50, 50, GREEN, false},
	{8820, 500, 50, 50, GREEN, false},
	{8870, 500, 50, 50, GREEN, false},
	{8920, 500, 50, 50, GREEN, false},
	{8970, 500, 50, 50, GREEN, false},

	// Tercer escal�n (7 bloques)
	{8670, 450, 50, 50, GREEN, false},
	{8720, 450, 50, 50, GREEN, false},
	{8770, 450, 50, 50, GREEN, false},
	{8820, 450, 50, 50, GREEN, false},
	{8870, 450, 50, 50, GREEN, false},
	{8920, 450, 50, 50, GREEN, false},
	{8970, 450, 50, 50, GREEN, false},

	// Cuarto escal�n (6 bloques)
	{8720, 400, 50, 50, GREEN, false},
	{8770, 400, 50, 50, GREEN, false},
	{8820, 400, 50, 50, GREEN, false},
	{8870, 400, 50, 50, GREEN, false},
	{8920, 400, 50, 50, GREEN, false},
	{8970, 400, 50, 50, GREEN, false},

	// Quinto escal�n (5 bloques)
	{8770, 350, 50, 50, GREEN, false},
	{8820, 350, 50, 50, GREEN, false},
	{8870, 350, 50, 50, GREEN, false},
	{8920, 350, 50, 50, GREEN, false},
	{8970, 350, 50, 50, GREEN, false},

	// Sexto escal�n (4 bloques)
	{8820, 300, 50, 50, GREEN, false},
	{8870, 300, 50, 50, GREEN, false},
	{8920, 300, 50, 50, GREEN, false},
	{8970, 300, 50, 50, GREEN, false},

	// S�ptimo escal�n (3 bloques)
	{8870, 250, 50, 50, GREEN, false},
	{8920, 250, 50, 50, GREEN, false},
	{8970, 250, 50, 50, GREEN, false},

	// Octavo escal�n (2 bloques)
	{8920, 200, 50, 50, GREEN, false},
	{8970, 200, 50, 50, GREEN, false},

	//Flag
	{9375, 550, 50, 50, GREEN, false },

	//Cueba escenario
		//Suelo
	{ -112, -500, 850, 100, BLACK, false },
	//Paredes-
	{ -112, -1000, 50, 350, BLACK, false },
	{ 688, -1000, 50, 350, BLACK, false },

	//techo
	{ 88, -1000, 350, 50, BLACK, false },//1

	//Zona monedas
	{ 88, -650, 350, 150, BLACK, false },//1
	//tuberia
	{ 578, -600, 100, 100, GREEN, false },//1
	};