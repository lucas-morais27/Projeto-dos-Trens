#include "trem.h"
#include <QtCore>
#include <QSemaphore>

using namespace std;

/*
 * Esquema das regiões críticas.

                _____#_______________
                |         |         |
                |         0         #
            ____|_1_____2_|_3_____4_|____
            |        |         |        |   
            #        5         6        #
            |________|____#____|________|
*/


QMutex regiao_critica[7];
QMutex mutex_auxiliar;
QSemaphore area0(2); //composta pelas regiões críticas 0, 2 e 3.
QSemaphore area1(2); //composta pelas regiões críticas 1, 2 e 5.
QSemaphore area2(2); //composta pelas regiões críticas 3, 4 e 6.
QSemaphore conj1(3); //conjunto de área 0 com área 1.
QSemaphore conj2(3); //conjunto de área 0 com área 2.
QSemaphore geral(4); //união dos dois conjuntos acima.

bool auxilia_teste[5]; //auxilia as verificações dos semáforos.
bool stoped; //auxilia o comando de entrada de um trem em uma região crítica.

//Construtor
Trem::Trem(int ID, int x, int y) {
    this->ID = ID;
    this->x = this->x_i = x;
    this->y = this->y_i = y;
    this->velocidade = 200; 
    stoped = false;
    this->area = -1;
}

//Função a ser executada após executar trem->START
void Trem::run() {
    while(true){

        // Seta o nº de recursos disponíveis nos semáforos.
        this->areas[0] = area0.available();
        this->areas[1] = area1.available();
        this->areas[2] = area2.available(); 
        this->areas[3] = conj1.available();
        this->areas[4] = conj2.available();
        this->areas[5] = geral.available();

        switch(ID){
        case 1: //Trem 1
            if (x == x_i + 70 && y == y_i) { //entrando na região 0
                mutex_auxiliar.lock(); //Lock
                auxilia_teste[5] = geral.tryAcquire(1); //tenta adquirir recursos guardados pelo semáforo.

                if (auxilia_teste[5]) { //verifica se pode entrar na regiao geral.
                    auxilia_teste[4] = conj1.tryAcquire(1); 

                    if (auxilia_teste[4]) { //verifica se pode entrar no conjunto 1.
                        auxilia_teste[3] = conj2.tryAcquire(1);

                        if (auxilia_teste[3]) { //verifica se pode entrar no conjunto 2.
                            auxilia_teste[2] = area1.tryAcquire(1);

                            if (auxilia_teste[2]) { //verifica se pode entrar área 1.
                                auxilia_teste[1] = regiao_critica[0].try_lock(); //tenta bloquear o mutex da regiao 0.

                                if (auxilia_teste[1]) { //se o mutex da regiao crítica 0 estiver bloqueado, ele entra e a região fica inacessível.
                                    stoped = false;
                                } else {
                                    geral.release(1); //libera os recursos guardados pelo semáforo.
                                    conj1.release(1);
                                    conj2.release(1);
                                    area1.release(1);
                                    stoped = true;
                                }
                            } else {
                                geral.release(1);
                                conj1.release(1);
                                conj2.release(1);
                                stoped = true;
                            }
                        } else {
                            geral.release(1);
                            conj1.release(1);
                            stoped = true;
                        }
                    } else {
                        geral.release(1);
                        stoped = true;
                    }
                } else {
                    stoped = true;
                }
                mutex_auxiliar.unlock(); //Unlock
                
            } else if (x == 240 && y < 140) { //entrando na região 2
                
            } else if (x > 110 && y == 140) { //entrando na região 1
                
            } else { //saindo da região 1
                
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (x < 240 && y == 40) { //Direita
                x+=10;
            } else if (x == 430 && y < 400) { // Baixo
                y+=10;
            } else if (x > 300 && y == 400) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (x < 240 && y == 40) { //Direita
                x+=10;
            } else if (x == 430 && y < 400) { // Baixo
                y+=10;
            } else if (x > 300 && y == 400) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (x < 240 && y == 40) { //Direita
                x+=10;
            } else if (x == 430 && y < 400) { // Baixo
                y+=10;
            } else if (x > 300 && y == 400) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (x < 240 && y == 40) { //Direita
                x+=10;
            } else if (x == 430 && y < 400) { // Baixo
                y+=10;
            } else if (x > 300 && y == 400) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        
        if (stoped == false && velocidade < 200) {
            if (x < x_i + 70 && y == y_i) { //Direita
                x+=10;
            } else if (x == x_i + 70 && y < y_i + 100) { // Baixo
                y+=10;
            } else if (x > x_i - 60 && y == y_i + 100) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
        }

        msleep(velocidade);

    }
}

void Trem::set_velocidade(int velocidade){
    this->velocidade = velocidade;
}