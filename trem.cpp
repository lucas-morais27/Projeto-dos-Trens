#include "trem.h"
#include <QtCore>

//Construtor
Trem::Trem(int ID, int x, int y) {
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 100;
}

//Função a ser executada após executar trem->START
void Trem::run() {
    while(true){
        switch(ID){
        case 1: //Trem 1
            if (x < 430 && y == 300 ) //Direita
                x+=10;
            else if (x == 430 && y < 400) // Baixo
                y+=10;
            else if (x > 300 && y == 400) // Esquerda
                x-=10;
            else // Cima
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (x < 560 && y == 300 ) //Direita
                x+=10;
            else if (x == 560 && y < 400) // Baixo
                y+=10;
            else if (x > 430 && y == 400) // Esquerda
                x-=10;
            else // Cima
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (x < 360 && y == 400 ) //Direita
                x+=10;
            else if (x == 360 && y < 500) // Baixo
                y+=10;
            else if (x > 230 && y == 500) // Esquerda
                x-=10;
            else // Cima
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (x < 500 && y == 400 ) //Direita
                x+=10;
            else if (x == 500 && y < 500) // Baixo
                y+=10;
            else if (x > 360 && y == 500) // Esquerda
                x-=10;
            else // Cima
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (x < 630 && y == 400 ) //Direita
                x+=10;
            else if (x == 630 && y < 500) // Baixo
                y+=10;
            else if (x > 500 && y == 500) // Esquerda
                x-=10;
            else // Cima
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}