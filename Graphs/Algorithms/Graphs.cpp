#include "Graphs.h"


int wcharToInt(WCHAR *c) {
	int num = 0;
	bool negative = false;
	for (int i = 0; i < lstrlen(c); i++) {
		if ((int)c[i] == 45) {
			negative = true;
			continue;
		}
		num *= 10;
		num += (int)c[i];
		num -= 48;
	}
	if (negative)
		num *= -1;
	return  num;
}

Graphs::Graphs() {
	nAristas = nVertices = 0;
	cont = 1;
	topologia = orden = 0;
	vertices = 0;
	aristas = distancias = 0;
	marca = 0;
	cc = 0;
	inc1 = inc2 = 0;
}

Graphs::~Graphs(){

}

int Graphs::contarAristas() {
	int cant = 0;
	for (int i = 0; i < nVertices; i++) {
		for (int j = 0; j < nVertices; j++) {
			if (aristas[i][j] == 1)
				cant++;
		}
	}
	return cant;
}

bool Graphs::agregar(WCHAR *vertice, int x, int y) {
	if (buscarVertice(vertice) != -1)
		return false;
	WCHAR **verticesTmp = new WCHAR*[nVertices + 1];
	circles *tmpCC = new circles[nVertices + 1];
	for (int i = 0; i < nVertices; i++) {
		verticesTmp[i] = vertices[i];
		tmpCC[i] = cc[i];
	}
	verticesTmp[nVertices] = new WCHAR(vertice[0]);
	if (vertices != 0)
		delete vertices;
	vertices = verticesTmp;

	if (cc != 0)
		delete cc;
	cc = tmpCC;
	cc[nVertices].vertice = new WCHAR(vertice[0]);
	cc[nVertices].x = x;
	cc[nVertices].y = y;

	int **tmpAristas = new int*[nVertices + 1];
	int **tmpDistancias = new int*[nVertices + 1];

	for (int i = 0; i < nVertices; i++) {
		tmpAristas[i] = aristas[i];
		tmpDistancias[i] = distancias[i];
	}

	if (aristas != 0)
		delete aristas;
	if (distancias != 0)
		delete distancias;
	distancias = tmpDistancias;
	aristas = tmpAristas;

	for (int i = 0; i < nVertices; i++) {
		int *tmpFila = new int[nVertices + 1];
		int *tmpFilaD = new int[nVertices + 1];
		for (int x = 0; x < nVertices; x++) {
			tmpFila[x] = aristas[i][x];
			tmpFilaD[x] = distancias[i][x];
		}
		if (aristas[i] != 0)
			delete aristas[i];
		if (distancias[i] != 0)
			delete distancias[i];
		tmpFila[nVertices] = 0;
		tmpFilaD[nVertices] = INF;
		aristas[i] = tmpFila;
		distancias[i] = tmpFilaD;

	}
	aristas[nVertices] = new int[nVertices + 1];
	distancias[nVertices] = new int[nVertices + 1];

	for (int i = 0; i < nVertices + 1; i++) {
		aristas[nVertices][i] = 0;
		distancias[nVertices][i] = INF;
	}

	nVertices++;
	return true;
}

bool Graphs::borrar(WCHAR *vertice) {
		int verticePos = buscarVertice(vertice);
		if (verticePos != -1) {
			if (nVertices - 1 == 0) {
				delete[] aristas;
				delete[] vertices;
				delete[] cc;
				delete[] distancias;
				distancias = 0;
				aristas = 0;
				vertices = 0;
				cc = 0;
			}
			else {
				WCHAR **tmpVertices = new WCHAR*[nVertices - 1];
				circles *tmpCC = new circles[nVertices - 1];
				int pos = 0;
				for (int i = 0; i < nVertices; i++) {
					if (i != verticePos) {
						tmpVertices[pos] = vertices[i];
						tmpCC[pos] = cc[i];
						pos++;
					}
				}
				pos = 0;
				delete[] vertices;
				delete[] cc;
				vertices = tmpVertices;
				cc = tmpCC;

				int **tmpAristas = new int*[nVertices - 1];
				int **tmpDistancias = new int*[nVertices - 1];
				for (int i = 0; i < nVertices; i++) {
					if (i != verticePos) {
						tmpAristas[pos] = aristas[i];
						tmpDistancias[pos] = distancias[i];
						pos++;
					}
				}

				int pos2 = 0;
				for (int i = 0; i < nVertices; i++) {
					if (i != verticePos) {
						pos = 0;
						int *tmpFila = new int[nVertices - 1];
						int *tmpFilaD = new int[nVertices - 1];
						for (int x = 0; x < nVertices; x++) {
							if (x != verticePos) {
								tmpFila[pos] = aristas[i][x];
								tmpFilaD[pos] = distancias[i][x];
								pos++;
							}
						}
						delete aristas[i];
						delete distancias[i];
						tmpAristas[pos2] = tmpFila;
						tmpDistancias[pos2] = tmpFilaD;
						pos2++;
					}
				}
				delete[]aristas;
				delete[]distancias;
				aristas = tmpAristas;
				distancias = tmpDistancias;
			}
			nVertices--;
			return true;
		}
		return false;
	}

bool Graphs::agregarArista(WCHAR *origen, WCHAR *destino, WCHAR *distancia) {
	if (origen[0] == destino[0])
		return false;
	int origin = buscarVertice(origen);
	int destination = buscarVertice(destino);
	if (origin != -1 && destination != -1) {
		aristas[origin][destination] = 1;
		aristas[destination][origin] = 1;

		int *tmpInc1 = new int[nAristas + 1];
		int *tmpInc2 = new int[nAristas + 1];

		for (int i = 0; i < nAristas; i++) {
			tmpInc1[i] = inc1[i];
			tmpInc2[i] = inc2[i];
		}

		if (inc1) {
			delete inc1;
			delete inc2;
		}
		inc1 = tmpInc1;
		inc2 = tmpInc2;

		inc1[nAristas] = origin;
		inc2[nAristas] = destination;

		nAristas++;

		distancias[origin][destination] = wcharToInt(distancia);
		distancias[destination][origin] = wcharToInt(distancia);
		return true;
	}
	return false;
}

bool Graphs::borrarArista(WCHAR *origen, WCHAR *destino) {
	if (origen[0] == destino[0])
		return false;
	int origin = buscarVertice(origen);
	int destination = buscarVertice(destino);
	if (origin != -1 && destination != -1) {
		aristas[origin][destination] = 0;
		distancias[origin][destination] = INF;

		int *tmpInc1 = new int[nAristas - 1];
		int *tmpInc2 = new int[nAristas - 1];
		int pos = 0;
		for (int i = 0; i < nAristas; i++) {
			if (!((inc1[pos] == origin && inc2[pos] == destination) || (inc1[pos] == destination && inc2[pos] == origin))) {
				tmpInc1[pos] = inc1[pos];
				tmpInc2[pos] = inc2[pos];
				pos++;
			}
		}
		delete[] inc1;
		delete[] inc2;
		inc1 = tmpInc1;
		inc2 = tmpInc2;

		return true;
	}
	return false;
}

int Graphs::buscarVertice(WCHAR *vertice) {
	for (int i = 0; i < nVertices; i++)
		if (vertices[i][0] == vertice[0])
			return i;
	return -1;
}

void Graphs::imprimir() {
	for (int i = 0; i < nVertices; i++)
		cout << vertices[i] << " ";
	cout << endl;

	for (int i = 0; i < nVertices; i++) {
		for (int x = 0; x < nVertices; x++) {
			cout << aristas[i][x];
		}
		cout << endl;
	}
}

bool Graphs::buscarCirculo(int x, int y, circles *LPCIRCLE) {
	for (int i = 0; i < nVertices; i++) {
		if ((cc[i].x >= x + 15 && cc[i].y >= y + 15) && (cc[i].x <= x + 40 && cc[i].y <= y + 40)) {
			LPCIRCLE->x = cc[i].x;
			LPCIRCLE->y = cc[i].y;
			return true;
		}
	}
	return false;
}

bool Graphs::esAciclico() {
	crearMarcas();
	aciclico = true;
	for (int i = 0; i < nVertices; i++) {
		crearMarcas();
		bpfAciclico(i);
	}
	delete[] marca;
	return aciclico;
}

void Graphs::crearMarcas() {
	marca = new bool[nVertices];
	for (int i = 0; i < nVertices; i++)
		marca[i] = false;
}

void Graphs::bpfAciclico(int vertice) {
	marca[vertice] = true;
	for (int i = 0; i < nVertices; i++) {
		if (aristas[vertice][i] == 1) {
			if (marca[i]) {
				aciclico = false;
				return;
			}
			bpfAciclico(i);
		}
	}
	marca[vertice] = false;
}

void Graphs::recursoTopologico() {
	if (topologia != 0)
		delete topologia;
	topologia = new int[nVertices];
	for (int i = 0; i < nVertices; i++)
		topologia[i] = 0;
	cont = 1;
	crearMarcas();
	for (int i = 0; i < nVertices; i++) {
		if (!marca[i])
			bpfTopologico(i);
	}
	delete[]marca;
}

void Graphs::bpfTopologico(int vertice) {
	marca[vertice] = true;
	for (int i = 0; i < nVertices; i++) {
		if (aristas[vertice][i] == 1) {
			if (!marca[i])
				bpfTopologico(i);
		}
	}
	topologia[vertice] = cont;
	cont++;
}

int* Graphs::componentesFuertes() {
	int *componentes = new int[nVertices];
	for (int i = 0; i < nVertices; i++)
		componentes[i] = 0;
	recursoTopologico();
	int **aristasInvertidas = invertirGrafo();
	encontrarComponentes(componentes, aristasInvertidas);
	return componentes;
}

void Graphs::algoritmoFloyd(Floyd *floyd) {
	floyd->distancias = new int*[nVertices];
	floyd->iteraciones = new int*[nVertices];
	for (int i = 0; i < nVertices; i++) {
		floyd->distancias[i] = new int[nVertices];
		floyd->iteraciones[i] = new int[nVertices];
	}
	for (int x = 0; x < nVertices; x++) {
		for (int y = 0; y < nVertices; y++) {
			if (x == y)
				floyd->distancias[x][y] = floyd->iteraciones[x][y] = EMPTY;
			else {
				if (aristas[x][y] == 1)
					floyd->distancias[x][y] = distancias[x][y];
				else
					floyd->distancias[x][y] = INF;
				floyd->iteraciones[x][y] = y + 1;
			}
		}
	}

	for (int i = 0; i < nVertices; i++) {
		for (int y = 0; y < nVertices; y++) {
			if (y == i)
				continue;
			for (int x = 0; x < nVertices; x++) {
				if (x == i || x == y)
					continue;
				if (floyd->distancias[y][i] != INF && floyd->distancias[i][x] != INF) {
					if (floyd->distancias[y][x] == INF) {
						floyd->distancias[y][x] = floyd->distancias[y][i] + floyd->distancias[i][x];
						floyd->iteraciones[y][x] = i + 1;
					}
					else if (floyd->distancias[y][i] + floyd->distancias[i][x] < floyd->distancias[y][x]) {
						floyd->distancias[y][x] = floyd->distancias[y][i] + floyd->distancias[i][x];
						floyd->iteraciones[y][x] = i + 1;
					}
				}
			}
		}
	}

}

void Graphs::algoritmoDijkstra(Dijkstra *dijkstra) {
	for (int i = 0; i < nVertices; i++) {
		dijkstra[i].iteracion = 0;
		dijkstra[i].distancia = 0;
		dijkstra[i].procede = 0;
	}
	crearMarcas();
	int iteracion = 1;
	marca[0] = true;
	bpfDijkstra(0, dijkstra, iteracion);
	delete[]marca;
}

void Graphs::bpfDijkstra(int vertice, Dijkstra *dijkstra, int iteracion) {
	for (int i = 0; i < nVertices; i++) {
		if ((aristas[vertice][i] == 1 && !marca[i])) {
			marca[i] = true;
			if (dijkstra[i].iteracion == 0 || distancias[vertice][i] + dijkstra[vertice].distancia < dijkstra[i].distancia) {
				dijkstra[i].iteracion = iteracion;
				dijkstra[i].distancia = distancias[vertice][i] + dijkstra[vertice].distancia;
				dijkstra[i].procede = vertice;
			}
			bpfDijkstra(i, dijkstra, iteracion + 1);
			marca[i] = false;
		}
	}
}

void Graphs::encontrarComponentes(int *componentes, int ** aristasInvertidas) {
	crearMarcas();
	int cont = nVertices;
	for (int i = 0; i < nVertices; i++) {
		int cont2 = encontrarCont(cont);
		if (!marca[cont2]) {
			componentes[cont2] = 1;
			bpfComponentes(cont2, aristasInvertidas);
		}
		cont--;
	}
}

void Graphs::bpfComponentes(int vertice, int **aristasInvertidas) {
	marca[vertice] = true;
	for (int i = 0; i < nVertices; i++) {
		if (aristasInvertidas[vertice][i] == 1) {
			if (!marca[i]) {
				bpfComponentes(i, aristasInvertidas);
			}
		}
	}
}

int Graphs::encontrarCont(int cont) {
	for (int i = 0; i < nVertices; i++) {
		if (topologia[i] == cont)
			return i;
	}
	return -1;
}

int** Graphs::invertirGrafo() {
	int **aristasInvertidas = new int*[nVertices];
	for (int i = 0; i < nVertices; i++)
		aristasInvertidas[i] = new int[nVertices];
	for (int x = 0; x < nVertices; x++) {
		for (int y = 0; y < nVertices; y++)
			aristasInvertidas[y][x] = aristas[x][y];
	}
	return aristasInvertidas;
}