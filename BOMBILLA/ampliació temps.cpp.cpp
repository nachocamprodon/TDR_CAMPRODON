#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono> //NOU!
#include <thread> //NOU!
using namespace std;
using namespace std::this_thread;     // NOU!
using namespace std::chrono_literals; // NOU!

/* Variables globals: les hem declarat fora del main i de totes les funcions perqu� les necessitem al llarg del tot
*  el programa. �s recomanable utilitzar el m�nim de variables globals possible, per evitar errors dif�cils de detec-
*  tar al llarg del codi. */
const int FILES = 10; //'const' �s una cl�usula de seguretat per evitar que aquesta variable es canvi� al llarg del programa.
const int COLUMNES = 10; //'const' �s una cl�usula de seguretat per evitar que aquesta variable es canvi� al llarg del programa.
int posicioSortidaX;
int posicioSortidaY;
int trampa1X, trampa1Y; //NOU!
int trampa2X, trampa2Y; //NOU!

// SENSE NOVETATS
void generarLaberint(int& posicioJugadorX, int& posicioJugadorY) {
    srand(time(0)); // Inicialitzar la llavor per a la generaci� aleat�ria
    //Utilitzeu la funci� rand(); per crear un nombre aleatori.
    int r = rand(); //r �s un nombre aleatori
    bool gL=false;
    //generar posicions aleatorias actualitzat
    while(!gL){
        posicioJugadorX = rand() % FILES;
        posicioJugadorY = rand() % COLUMNES;
        posicioSortidaX = rand() % FILES;
        posicioSortidaY = rand() % COLUMNES;
        gL = (posicioJugadorX, posicioJugadorY) != (posicioSortidaX, posicioSortidaY);
    }

}

// he afegit tempsRestant per mostrar el crono a la pantalla
void mostrarLaberint(int& posicioJugadorX, int& posicioJugadorY, int tempsRestant) {
    cout << "Temps restant: " << tempsRestant << " segons" << endl; // cada vegada que es mou el jugador s'actualitza el temps
    // es el que m�s m'ha costat perque no se fer-los
    //imprimeix el laberint
    for (int i = 0; i < FILES; i++) {
        for (int j = 0; j < COLUMNES; j++) {
            if (i == posicioJugadorX and j == posicioJugadorY) {
                cout << " J";
            }
            else if (i == posicioSortidaX and j == posicioSortidaY) {
                cout << " S";
            }
            else {
                cout << " #";
            }
        }
        cout << endl;
    }
}
// SENSE NOVETATS
bool movimentValid(int x, int y) {
    //comprova si el moviment est� dins del taulell
    if (x>=0 and x<FILES and y>=0 and y<COLUMNES) {
        return true;
    } else {
        return false;
    }
}



//  SENSE NOVETATS
void moureJugador(char direccio, int& posicioJugadorX, int& posicioJugadorY) {
    //per moure la J depenent de la tecla que �s premi
    //en un primer moment ho havia fet de tal manera que funciones amb altres controls (ASDW en comptes WASD) simplement ho he canviat
    switch (direccio) {
        case 'D':
        case 'd':
            if (movimentValid(posicioJugadorX, posicioJugadorY+1)) {
                posicioJugadorY++;
            }
            break;
        case 'W':
        case 'w':
            if (movimentValid(posicioJugadorX -1, posicioJugadorY)) {
                posicioJugadorX--;
            }
            break;
        case 'A':
        case 'a':
            if (movimentValid(posicioJugadorX, posicioJugadorY-1)) {
                posicioJugadorY--;
            }
            break;
        case 'S':
        case 's':
            if (movimentValid(posicioJugadorX +1, posicioJugadorY)) {
                posicioJugadorX++;
            }
            break;

    }
}


// (CONSULTEU NOVETATS) ESPECIFICACI� camiMesCurt():
// Pre: 0 <= x < FILES; 0 <= y < COLUMNES; les variables posicioSortidaX, posicioSortidaY han estat inicialitzades en
//      valors entre 0 i FILES-1, i 0 i COLUMNES-1
// Post: Es retorna la quantitat de moviments que s'han de fer en el cam� m�s curt per arribar a la posici� de sortida
//       des de la posici� (x, y) tenint en compte la posici� de les trampes (NOU!)
int camiMesCurt(int x, int y) {
    //calcula la dist�ncia al cam� m�s curt per arribar a la posici�
 int distancia = abs(x - posicioSortidaX) + abs(y - posicioSortidaY);

    // Si la posici� �s la mateixa que alguna de les dues trampes, s'incrementa la dist�ncia.
    if ((x == trampa1X and y == trampa1Y) or (x == trampa2X and y == trampa2Y)) {
        distancia += 5; // per evitar que siigui la mateixa posici�
    }

    return distancia;
}

// NOU: ESPECIFICACI� crearTrampes():
// Pre: 0 <= posicioJugadorX < FILES, 0 <= posicioJugadorY < COLUMNES, i 0 <= posicioSortidaX < FILES,
//      0 <= posicioSortidaY < COLUMNES
// Post: trampaX i trampaY prenen un valor dins de les ubicacions possibles (consulteu l'enunciat - fig. 3).
//       Es compleix que: (trampaX, trampaY) != (posicioJugadorX, posicioJugadorY) i que
//       (trampaX, trampaY) != (posicioSortidaX, posicioSortidaY). Programeu aquesta comprovaci� de forma recursiva.
void crearTrampes(int& posicioJugadorX, int& posicioJugadorY, int& trampaX, int& trampaY) {
    srand(time(0));
    // aix� serveix per fer que mes o menys  les trampes estiguin entre la J i la S
    int minX = min(posicioJugadorX, posicioSortidaX);
    int maxX = max(posicioJugadorX, posicioSortidaX);
    int minY = min(posicioJugadorY, posicioSortidaY);
    int maxY = max(posicioJugadorY, posicioSortidaY);

    // Generar trampa1
    trampa1X = minX + rand() % (maxX - minX + 1);
    trampa1Y = minY + rand() % (maxY - minY + 1);
    while (trampa1X == posicioJugadorX and trampa1Y == posicioJugadorY or trampa1X == posicioSortidaX and trampa1Y == posicioSortidaY) {
        trampa1X = minX + rand() % (maxX - minX + 1);
        trampa1Y = minY + rand() % (maxY - minY + 1);
    }
    // Generar trampa2
    trampa2X = minX + rand() % (maxX - minX + 1);
    trampa2Y = minY + rand() % (maxY - minY + 1);
    while (trampa2X == posicioJugadorX and trampa2Y == posicioJugadorY or trampa2X == posicioSortidaX and trampa2Y == posicioSortidaY or (trampa2X == trampa1X && trampa2Y == trampa1Y)) {
        trampa2X = minX + rand() % (maxX - minX + 1);
        trampa2Y = minY + rand() % (maxY - minY + 1);
    }
}
// ESPECIFICACI� mostrarTrampes():
// Pre-Post: aquesta funci� fa el mateix que mostrarLaberint(), per� tamb� mostra les posicions de les trampes amb el
//           s�mbol "O".
void mostrarTrampes(int& posicioJugadorX, int& posicioJugadorY) {
    for (int i = 0; i < FILES; i++) {
        for (int j = 0; j < COLUMNES; j++) {
            if (i == posicioJugadorX and j == posicioJugadorY) {
                cout << " J";
            }
            else if (i == posicioSortidaX and j == posicioSortidaY) {
                cout << " S";
            }
            else if ((i == trampa1X and j == trampa1Y) or (i == trampa2X and j == trampa2Y)) {
                cout << " O";
            }
            else {
                cout << " #";
            }
        }
        cout << endl;
    }
}

int main() {
    int X, Y;
    int s = 10;
    generarLaberint(X, Y);
    //crear trampes
    crearTrampes(X, Y, trampa1X, trampa1Y);
    crearTrampes(X, Y, trampa2X, trampa2Y);
    mostrarTrampes(X,Y);
    //esperar 2 segons
    this_thread::sleep_for(chrono::seconds(2));
    system("cls");

    int movimentsRealitzats = 0;
    int maximMoviments = 15; // l�mit de moviments
    //trobat a cppreference i Antonio Jos� Checa Bustos "C�mo usar la librer�a Chrono en C++"
    auto start = chrono::steady_clock::now();
    while (true) {
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        int tempsRestant = 10 - chrono::duration_cast<chrono::seconds>(diff).count();
        mostrarLaberint(X,Y, tempsRestant);
        char d;
        cin >> d;
        moureJugador(d,X,Y);
        movimentsRealitzats++;// per el l�mit de moviments, incrementa cada vegada que s'introdueix un car�cter
        //temporitzador
        if (tempsRestant <= 0) {

            cout << "Temps limit superat! Game Over:(" << endl;
            break;
        }
        // Comprova si el jugador ha arribat a la sortida
        int distancia = camiMesCurt(X,Y);
        if (distancia == 0) {
            cout << "Felicitats! Has trobat la sortida!" << endl;
            break;
        }

        // Comprova si el jugador ha caigut en una trampa
        if ((X == trampa1X and Y == trampa1Y) or (X == trampa2X and Y == trampa2Y)) {
            cout << "Has caigut en una trampa! Game Over:(" << endl;
            break;
        }

        // Comprova si el jugador ha superat el l�mit de moviments
        if (movimentsRealitzats >= maximMoviments) {
            cout << "Has fet massa moviments! Game Over:(" << endl;
            break;
        }

        sleep_for(1ms); // Pausa per actualitzar la sortida
        system("cls");    // Buida la pantalla per a la propera iteraci�
    }
}
