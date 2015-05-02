/*
	The old class and logic implemented in the previous graph
	project. This code might get a code refactor for more code
	readability after the visual layer is completely done.
*/

#pragma once

#include <iostream>
#include <Windows.h>

using namespace std;
#define INF -1
#define EMPTY -2

int wcharToInt(WCHAR *);

struct circles {
	int x, y;
	WCHAR *vertice;
};

struct Floyd {
	int **distancias;
	int **iteraciones;
};

struct Dijkstra {
	int iteracion, distancia, procede;
};

class Graphs {
public:
	circles* cc;
	WCHAR**  vertices;
	bool*    marca;
	int**    aristas;
	int**    distancias;
	int*     topologia;
	int*	 orden;
	int*     inc1;
	int*     inc2;
	bool     aciclico;
	int      nVertices;
	int      nAristas;
	int      cont;
	
	Graphs();
	~Graphs();

	int  contarAristas();
	bool agregar(WCHAR*, int, int);
	bool borrar(WCHAR*);
	bool agregarArista(WCHAR*, WCHAR*, WCHAR*);
	bool borrarArista(WCHAR*, WCHAR*, WCHAR*);
	int buscarVertice(WCHAR *vertice);
	void imprimir();
	bool buscarCirculo(int, int, circles*);
	bool esAciclico();
	void crearMarcas();
	void bpfAciclico(int vertice);
	void recursoTopologico();
	void bpfTopologico(int);
	int* componentesFuertes();
	void algoritmoFloyd(Floyd*);
	void algoritmoDijkstra(Dijkstra*);

private:
	void  bpfDijkstra(int, Dijkstra*, int iteracion);
	void  encontrarComponentes(int*, int**);
	void  bpfComponentes(int, int**);
	int   encontrarCont(int);
	int** invertirGrafo();

};