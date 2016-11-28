#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import math
import matplotlib.pyplot as plt
import gc

# vec_w es el vector de pesos de cada uno de los elementos
# vec_w es el vector de valores de cada uno de los elementos
# valores_aux es el vector de valores "redondeados"
vec_w = []
vec_v = []
valores_aux = []

# optVal es la solución óptima de la instancia según Pisinger
# vec_selec es vector de 1s y 0s según si se elige o no ese elem. en
# la solución óptima según Pisinger
optVal = 0
vec_selec = []

# seleccionados es el vector de 1s y 0s según si se elige o no ese elem.
# según nuestro algoritmo resuelve mochila
seleccionados = []
# Vector de soluciones parciales mochila(j, w)
soluciones = []

def parsear_instancia(f):	
	f.readline() # Leo el título de la instancia
	cantElem =  int(f.readline().split()[1]) # Leo C
	capMochila = int(f.readline().split()[1]) # Leo W
	valOpt = int(f.readline().split()[1]) # Leo la sol. optima
	f.readline() # Leo el tiempo que tardo la instancia
	i = 0
	while (i < cantElem):
		lineAct = f.readline().split(',')
		vec_v.append(int(lineAct[1]))
		vec_w.append(int(lineAct[2]))
		vec_selec.append(int(lineAct[3]))

		i += 1
	f.readline() # Leo línea de separación --------
	f.readline() # Leo la línea en blanco
	
	return (cantElem, capMochila, valOpt)
	
def solveMochila(cantElem, capMochila):
	for i in range(0, cantElem+1):
		for v in range(0, sum(valores_aux)+1):
			if(i == 0) and (v == 0):
				soluciones[i][v] = 0
			elif(i == 0):	
				soluciones[i][v] = 99999999
			elif(valores_aux[i-1] > v):
				soluciones[i][v] = soluciones[i-1][v]
			else:
				soluciones[i][v] = min(soluciones[i-1][v], vec_w[i-1] + soluciones[i-1][v-valores_aux[i-1]])
	# Devolver el maximo v tq soluciones[cantElem][v] <= capMochila
	maxV = -1
	for v in range (1, sum(valores_aux)+1):
		if (v > maxV) and (soluciones[cantElem][v] <= capMochila):
			maxV = v
	return maxV	
	
# Malo Tardos malo	
def solveMochila2(cantElem, capMochila):
	global valores_aux
	global soluciones
	global vec_w
	for i in range (0, cantElem+1):
		for v in range (0, sum(valores_aux)+1):
			soluciones[i][v] = 0
	for v in range(0, sum(valores_aux)+1):
		soluciones[0][v] = 999999999
	soluciones[0][0] = 0
	for i in range (1, cantElem+1):
		for v in range (1, sum(valores_aux[:i])+1):
			#print sum(valores_aux[:i])
			if v > sum(valores_aux[:i-1]):
				#print v
				#print sum(valores_aux[:i-1])
				soluciones[i][v] = vec_w[i-1] + soluciones[i-1][v]
			else:
				soluciones[i][v] = min(soluciones[i-1][v], vec_w[i-1] + soluciones[i-1][max(0, v-valores_aux[i-1])])
	# Devolver el maximo v tq soluciones[cantElem][v] <= capMochila
	maxV = -1
	for v in range (1, sum(valores_aux)+1):
		if (v > maxV) and (soluciones[cantElem][v] <= capMochila):
			maxV = v
	print maxV
	print soluciones[cantElem][maxV]
	print soluciones[cantElem][1280]

def mochilaAproximada(cantElem, capMochila, precision):
	global valores_aux
	global vec_v
	if(precision == 1) or (precision == 1.0):
		valores_aux = vec_v
		return solveMochila(cantElem, capMochila)
	valores_aux = list(vec_v)
	b = precision * float(max(vec_v)) / (2.0 * cantElem)
	for i in range(0, cantElem):
		valores_aux[i] = int(math.floor(valores_aux[i] / b))
	return solveMochila(cantElem, capMochila)

def devolverS(cantElem, maxV):
	for i in range(cantElem, -1, -1):
		if(soluciones[i][maxV] != soluciones[i-1][maxV]):
			seleccionados[i-1] = 1
			maxV -= valores_aux[i-1]

# De acá en adelante es la zona de tests

arch = "Mochila/Dificiles/"
arch += "knapPI_13_100_1000.csv"
f = open(arch, 'r')
dist_tiempos = []
tProm = 0.0
for instanciaAct in range (0, 50):
	vec_w = []
	vec_v = []
	cantElem = 0
	Wmochila = 0
	optVal = 0
	vec_selec = []
	seleccionados = []
	soluciones = []

	gc.collect()

	(cantElem, Wmochila, optVal) = parsear_instancia(f)
	for i in range(0, cantElem+1):
		dic = {}
		soluciones.append(dic)
	for i in range(0, cantElem):
		seleccionados.append(0)
		
	print "Instancia actual: ", instanciaAct + 1
	
	t0 = time.time()
	a = mochilaAproximada(cantElem, Wmochila, 1)
	t1 = time.time()
	
	devolverS(cantElem, a)
	
	a = 0
	for i in range (0, cantElem):
		if seleccionados[i]:
			a += vec_v[i]
			

	print "Solucion encontrada: ", a
	print "Solucion optima real: ", optVal
	deltaT = t1 - t0
	tProm += (deltaT * 0.01)
	print "Tiempo: ", deltaT 
	dist_tiempos.append(deltaT)
f.close()

print "Tiempo promedio: ", tProm

plt.hist(dist_tiempos, bins=15, histtype='stepfilled', color='b', stacked=True, alpha = 1.0, label="knapPI_13_50_1000")
plt.title("Dist. de tiempos")
plt.xlabel("Tiempo [seg.]")
plt.ylabel("Frecuencia")
plt.grid(True)
#plt.ylim(0, 9)
#plt.legend()
plt.show()

#	seleccionar_elementos(cantElem-1, Wmochila)
#	print "Combinación óptima encontrada: ", seleccionados == vec_selec
"""

vec_w = [1, 2, 3, 2, 3, 2]
vec_v = [10, 15, 24, 12, 15, 20]
cantElem = 6
Wmochila = 5

# Capacidad mochila: 4 => 1010 (40)
print "Pesos de los items:", vec_w
print "Valores de los items:", vec_v
print "Capacidad mochila:", Wmochila
for i in range(0, cantElem+1):
	dic = {}
	soluciones.append(dic)
for i in range(0, cantElem):
	seleccionados.append(0)

devolverS(len(vec_v), mochilaAproximada(len(vec_v), Wmochila, 0.01))
print "Elegir items:", seleccionados
"""

