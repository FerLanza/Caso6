// FlockCase.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//Maria Fernanda Lanza, 2018133074
//Importante los datos de las aves se deben copiar a un txt para hacer su lectura.
//Se programo en visual studio 2017 con la edicion OpenMP

#include <iostream>
#include <string>
#include <fstream>
#include "bird.h"

using namespace std;

const int array_size = 100000;		
const int quantityBirds = 15;				//Cantidad de pajaros , este debe ser igual al codigo en javascript 
int numBird = 0;							//Lleva el conteo de verdaderamente cuantos movimientos se registraron para no tocar basura 

string text[array_size];					//Array que guarda todo el texto
Bird * birdData[array_size];				//Array de objetos que guarda cada movimiento
double velocities[quantityBirds] = { 0.00};	//Array que guarda el promedio de las velocidades 
double distances[quantityBirds] = { 0.00 };	//Array que guarda las sumas de las distancias 

/*
Entrada:--
Salida: un array con las lineas del texto 
Funcion: leer y cargar el txt en un array por linea . Importante se debe cambiar la ubicacion del archivo 
*/
void loader() {
	int first = 0;

	ifstream myfile("C:/Users/ferla/source/repos/FlockCase/data.txt");			//Se debe cambiar 
	while (!myfile.eof())
	{
		getline(myfile, text[first]);				//Se carga cada linea en el array text
		numBird++;
		first++;
	}
}
/*
Entrada:--
Salida: un array con objetos 
Funcion: se procesan los datos sacados del texto, se crean objetos bird donde contiene id , su velocidad y distancia. Se decidio utilizar objetos para la permanencia de datos y cada uno 
de los movimitos.
*/
void objectCreator() {				
	int position = 0;				//Lleva el control de la posicion en el array de los objetos llamado birData
	string texto = "";				//Se saca el string correspondiente del array 
	int IdNum;						//Se guarda el id del pajaro

									//Se guarda como objetos 
	for (int begin = 0; begin < numBird; begin++) {
		birdData[position] = new Bird();
		int num = 12;				//Numero de caracteres que debe brincarse para extraer el numero correcto
		texto = "";					//Se inicializa el string 
		while (text[begin][num] != ' ') {
			texto += text[begin][num];		//Se guarda el id 
			num++;
		}
		IdNum = stoi(texto);				//Se mantiene como un int 
		birdData[position]->id = texto;
		begin++;
		texto = "";
		num = 12;
		while (text[begin][num] != ' ') {
			texto += text[begin][num];		//Se saca la velocidad 
			num++;
		}
		velocities[IdNum] = velocities[IdNum] + stod(texto);		//Se suma al acomulado que se lleva de las velocidades de cada pajaro
		birdData[position]->velocity = stod(texto);					//Se asigna al atributo velocidad la correspondiente
		begin++;
		texto = "";
		num = 12;
		while (text[begin][num] != ' ') {							//Se saca la distancia 
			texto += text[begin][num];
			num++;
		}
		distances[IdNum] = distances[IdNum] + stod(texto);			//Se suma al acomulado que se lleva de las distancias 
		birdData[position]->distance = stod(texto);					//Se asigna al atributo distancia la correspondiente
		position++;
	}
}
/*
Entrada: double velocidad 
Salida: id del pajaro 
Funcion: se busca el id del pajaro al que corresponde la velocidad
*/
int birdSearchVelocity(double pVelocity) {
	for (int i = 0; i < quantityBirds; i++) {
		if (velocities[i] == pVelocity) {
			return i;
		}
	}
}
/*
Entrada:double distancia
Salida: un array con objetos
Funcion: se busca el id del pajaro al que corresponde la distancia
*/
int birdSearchDistance(double pDistance) {
	for (int i = 0; i < quantityBirds; i++) {
		if (distances[i] == pDistance) {
			return i;
		}
	}
}
int main()
{	
	loader();
	objectCreator();
	
	//Variables declaradas para elaborar el ranking 
	double maxDistanceOne = 0 , maxDistanceTwo = 0;
	double minDistanceOne =100 , minDistanceTwo = 100;
	double maxVelocityOne = 0, maxVelocityTwo = 0;
	double minVelocityOne = 100, minVelocityTwo = 100;
	
	int firstPosition = 0;				//Variable inicial y privada para el parallel 
	#pragma omp parallel for private(firstPosition)
	{
		for (firstPosition = 0; firstPosition < quantityBirds; firstPosition++) { //Este for saca las respuestas del ranking 
			if (distances[firstPosition] > maxDistanceOne) {
				maxDistanceTwo = maxDistanceOne;								//Si la distancia es mayor a la seleccionada en la variable se cambia 
				maxDistanceOne = distances[firstPosition];
			}
			if (distances[firstPosition]< maxDistanceOne && distances[firstPosition] > maxDistanceTwo) {
				maxDistanceTwo = distances[firstPosition];						//Si la distancia es menor a la maxDistanceOne y mayor a la maxDistanceTwo se cambia 
			}
			if (velocities[firstPosition] > maxVelocityOne) {					//Si la velocidad es mayor a la maxVelocityOne se cambia 
				maxVelocityTwo = maxVelocityOne;
				maxVelocityOne = velocities[firstPosition];
			}
			if (velocities[firstPosition]< maxVelocityOne && velocities[firstPosition] > maxVelocityTwo) {
				maxVelocityTwo = velocities[firstPosition];						//Si la velocidad es menor a la maxVelocityOne y mayor a la maxVelocityTwo se cambia 
			}
			if (distances[firstPosition] < minDistanceOne) {
				minDistanceTwo = minDistanceOne;
				minDistanceOne = distances[firstPosition];						//Si la distancia es menor a la que se encuentra en la variable se cambia 
			}
			if (distances[firstPosition] > minDistanceOne && distances[firstPosition] < minDistanceTwo) {
				minDistanceTwo = distances[firstPosition];						
			}
			if (velocities[firstPosition] < minVelocityOne) {					//Si la velocidad es menor a la que se encuentra en la variable se cambia 
				minVelocityTwo = minVelocityOne;
				minVelocityOne = velocities[firstPosition];
			}
			if (velocities[firstPosition] > minVelocityOne && velocities[firstPosition] < minVelocityTwo) {
				minVelocityTwo = velocities[firstPosition];
			}
		}
	}
	cout << "Ranking \n";
	cout << "Top 2 de las distancias mayores:\n";
	cout << "1.La distancia del pajaro con el ID " + to_string(birdSearchDistance(maxDistanceOne)) + " color azul es de: " + to_string(maxDistanceOne) + "\n";
	cout << "2.La distancia del pajaro con el ID " + to_string(birdSearchDistance(maxDistanceTwo)) + " color rojo es de: " + to_string(maxDistanceTwo) + "\n";
	cout << "\n";
	cout << "Top 2 de las distancias menores: \n";
	cout << "1.La distancia del pajaro con el ID "+ to_string(birdSearchDistance(minDistanceOne))+ " color verde es de: " + to_string(minDistanceOne) + "\n";
	cout << "2.La distancia del pajaro con el ID " + to_string(birdSearchDistance(minDistanceTwo)) + " color amarillo es de: " + to_string(minDistanceTwo) + "\n";
	cout << "\n";
	cout << "Top 2 de las velocidades mayores: \n";
	cout << "1.La velocidad del pajaro con el ID " + to_string(birdSearchVelocity(maxVelocityOne)) + " color morado es de: " + to_string(maxVelocityOne) + "\n";
	cout << "2.La velocidad del pajaro con el ID " + to_string(birdSearchVelocity(maxVelocityTwo)) + " color rosado es de: " + to_string(maxVelocityTwo) + "\n";
	cout << "\n";
	cout << "Top 2 de las velocidades menores: \n";
	cout << "1.La velocidad del pajaro con el ID " + to_string(birdSearchVelocity(minVelocityOne)) + " color cafe es de: " + to_string(minVelocityOne) + "\n";
	cout << "2.La velocidad del pajaro con el ID " + to_string(birdSearchVelocity(minVelocityTwo)) + " color naranja es de: " + to_string(minVelocityTwo) + "\n";
	cout << "\n";

	//Esto es lo que se copia en el javascript en la parte del update para hacer el pintado
	cout <<
		"var maxDistanceOne = " << maxDistanceOne << ";\n" <<
		"var maxDistanceTwo = " << maxDistanceTwo << ";\n" <<
		"var minDistanceOne = " << minDistanceOne << ";\n" <<
		"var minDistanceTwo = " << minDistanceTwo << ";\n" <<
		"var maxVelocityOne = " << maxVelocityOne << ";\n" <<
		"var maxVelocityTwo = " << maxVelocityTwo << ";\n" <<
		"var minVeolocityOne = " << minVelocityOne <<";\n" <<
		"var minVelocityTwo = " << minVelocityTwo << ";\n" <<endl;
}

