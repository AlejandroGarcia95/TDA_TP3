#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Generador random de instancias de mochila
from __future__ import print_function
import random

# Parametros para la generacion de instancias random
cantInstancias = 100
cantElem = 200
valorMin = 1
valorMax = 100
pesoMin = 1600
pesoMax = 16000
Wmochila = 80000


f = open("Mochila/Dificiles/GeneradosRandom.csv", 'w')

for j in range(0, cantInstancias):
	print("GeneradosRandom_"+str(j+1), file=f)
	print("n", cantElem, file=f)
	print("c", Wmochila, file=f)
	print("z", 999999, file=f)
	print("time", 9999.99, file=f)

	for i in range (0, cantElem):
		linea = str(i+1) + ","
		linea += ( str(random.randint(valorMin, valorMax)) + ",")
		linea += ( str(random.randint(pesoMin, pesoMax)) + ",0")
		print(linea, file=f)
		
	print("-----", file=f)
	print("", file=f)
	

f.close()
