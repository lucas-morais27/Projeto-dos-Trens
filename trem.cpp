#include "trem.h"
#include <QtCore>
#include <QSemaphore>

using namespace std;

/*
 * Esquema das regiões críticas.

                _____#_______________
                |         |         |
                |    ⤵    0   ⤵    #
            ____|_1_____2_|_3_____4_|____
            |        |         |        |   
            #   ⤵   5    ⤵    6   ⤵   #
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
    this->x = x;
    this->y = y;
    this->velocidade = 100; 
    stoped = false;
    this->area = -1;
}

//Função a ser executada após executar trem->START
void Trem::run() {
    while(true) {

        // Seta o nº de recursos disponíveis nos semáforos.
        this->areas[0] = area0.available();
        this->areas[1] = area1.available();
        this->areas[2] = area2.available(); 
        this->areas[3] = conj1.available();
        this->areas[4] = conj2.available();
        this->areas[5] = geral.available();

        switch(ID) {
        case 1: //Trem 1
            if (x == 240 && y == 40) { //entrando na região 0
                mutex_auxiliar.lock(); //Lock
                auxilia_teste[1] = geral.tryAcquire(1); //tenta adquirir recursos guardados pelo semáforo.

                if (auxilia_teste[1]) { //verifica se pode entrar na regiao geral.
                    auxilia_teste[2] = conj1.tryAcquire(1); 

                    if (auxilia_teste[2]) { //verifica se pode entrar no conjunto 1.
                        auxilia_teste[3] = conj2.tryAcquire(1);

                        if (auxilia_teste[3]) { //verifica se pode entrar no conjunto 2.
                            auxilia_teste[4] = area0.tryAcquire(1);

                            if (auxilia_teste[4]) { //verifica se pode entrar área 1.
                                auxilia_teste[5] = regiao_critica[0].try_lock(); //tenta bloquear o mutex da regiao 0.

                                if (auxilia_teste[5]) { //se o mutex da regiao crítica 0 estiver bloqueado, ele entra e a região fica inacessível.
                                    stoped = false;
                                } else {
                                    geral.release(1); //libera os recursos guardados pelo semáforo.
                                    conj1.release(1);
                                    conj2.release(1);
                                    area0.release(1);
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
                
            } else if (x == 240 && y == 130) { //entrando na região 2
                mutex_auxiliar.lock(); //Lock
                auxilia_teste[1] = area1.tryAcquire(1); //tenta adquirir recursos guardados pelo semáforo.

                if (auxilia_teste[1]) { //verifica se pode entrar na área 2.
                    auxilia_teste[2] = regiao_critica[2].try_lock(); //tenta bloquear o mutex da regiao 2.

                    if (auxilia_teste[2]) { //se o mutex da regiao crítica 2 estiver bloqueado, ele entra e a região fica inacessível.
                        regiao_critica[0].unlock(); //Libera a região crítica 0.
                        area = 2;
                        stoped = false;
                    } else {
                        area1.release(1); //libera os recursos guardados pelo semáforo.
                        stoped = true;
                    }
                } else {
                    stoped = true;
                }
                mutex_auxiliar.unlock(); //Unlock
                
            } else if (x == 170 && y == 140) { //entrando na região 1
                regiao_critica[1].lock(); //Lock na região crítica 1.
                regiao_critica[2].unlock(); //Libera a região crítica 2.
                area = 1;
                mutex_auxiliar.lock(); //Lock
                area0.release(1); //libera os recursos guardados pelo semáforo.
                mutex_auxiliar.unlock(); //Unlock

            } else if (x == 110 && y == 140) { //saindo da região 1
                regiao_critica[1].unlock(); //Libera a região crítica 1.
                mutex_auxiliar.lock(); //Lock
                geral.release(1); //libera os recursos guardados pelo semáforo.
                conj2.release(1);
                conj1.release(1);
                area1.release(1);
                mutex_auxiliar.unlock(); //Unlock
            }
            break;

        case 2: //Trem 2
            if (x == 380 && y == 130) { //entrando na região 4
                mutex_auxiliar.lock(); //Lock
                auxilia_teste[1] = geral.tryAcquire(1);

                if (auxilia_teste[1]) {
                    auxilia_teste[2] = conj2.tryAcquire(1);

                    if (auxilia_teste[2]) {
                        auxilia_teste[3] = area2.tryAcquire(1);

                        if (auxilia_teste[3]) {
                            auxilia_teste[4] = regiao_critica[4].try_lock();

                            if (auxilia_teste[4]) {
                                area = 4;
                                stoped = false;
                            } else {
                                geral.release(1);
                                area2.release(1);
                                conj2.release(1);
                                stoped = true;
                            }
                        } else {
                            geral.release(1);
                            conj2.release(1);
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
            
            } else if (x == 310 && y == 140) {//Entrando na região 3
                mutex_auxiliar.lock();//Lock

                auxilia_teste[1] = conj1.tryAcquire(1);
                if (auxilia_teste[1]) {

                    auxilia_teste[2] = area0.tryAcquire(1);
                    if (auxilia_teste[2]) {

                        auxilia_teste[3] = regiao_critica[3].try_lock();
                        if (auxilia_teste[3]) {
                            area=3;
                            regiao_critica[4].unlock();
                            stoped = false;
                        } else {
                            area0.release(1);
                            conj1.release(1);
                            stoped = true;
                        }
                    } else {
                        conj1.release(1);
                        stoped = true;
                    }
                } else {
                    stoped = true;
                }

                mutex_auxiliar.unlock();//Lock

            } else if (x == 240 && y == 140) {//Entrando na região 0
                regiao_critica[0].lock();
                area=0;
                regiao_critica[3].unlock();
                mutex_auxiliar.lock();
                area2.release(1);
                mutex_auxiliar.unlock();

            } else if (x == 240 && y == 40) {//Saindo da região 0
                regiao_critica[0].unlock();
                mutex_auxiliar.lock();
                geral.release(1);
                area0.release(1);
                conj1.release(1);
                conj2.release(1);
                mutex_auxiliar.unlock();
            }
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
        
        /*
         * De acordo com o valor da variável auxiliar stoped
         * junto com a variável velocidade, o trem se move 
         * nas direções correspondente a sua posição atual.
         */
        if (stoped == false && velocidade < 200 && ID == 1) {
            if (x < 240 && y == 40) { //Direita
                x+=10;
            } else if (x == 240 && y < 140) { // Baixo
                y+=10;
            } else if (x > 110 && y == 140) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
        } else if (stoped == false && velocidade < 200 && ID == 2) {
            if (x < 380 && y == 40) { //Direita
                x+=10;
            } else if (x == 380 && y < 140) { // Baixo
                y+=10;
            } else if (x > 240 && y == 140) { // Esquerda
                x-=10;
            } else { // Cima
                y-=10;
            }
            emit updateGUI(ID,x,y);    //Emite um sinal
        }

        msleep(velocidade);

    }
}

void Trem::set_velocidade(int velocidade) {
    this->velocidade = velocidade;
}