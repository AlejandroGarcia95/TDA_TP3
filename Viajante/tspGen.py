#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Generador random de instancias de TSP
from __future__ import print_function
import numpy as np
import random

# Parametros para la generacion de instancias random
cantCiudades = 1000
maxX = 100
maxY = 100

archName = "random" + str(cantCiudades)
f = open(archName + ".txt", 'w')
ciudades = []

for i in range(0, cantCiudades):
	coordX = random.randint(0, maxX)
	coordY = random.randint(0, maxY)
	ciudades.append(np.array([coordX, coordY]))
		
for i in range(0, cantCiudades):
	cadenita = ""
	for j in range(0, cantCiudades):
		cadenita += "%.3f" % np.linalg.norm(ciudades[i]-ciudades[j])
		cadenita += " "
	print(cadenita, file=f)

f.close()	
