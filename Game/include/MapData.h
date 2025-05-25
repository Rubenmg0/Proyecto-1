#ifndef MAPDATA_H
#define MAPDATA_H

using namespace std;
#include <vector>
#include "raylib.h"
#include "Structures.h" 

extern vector<EnvElement> blocks;
extern vector<EnvElement> monedas;

void LoadMap();  // Función que inicializa todos los bloques

#endif
