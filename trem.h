#ifndef TREM_H
#define TREM_H

#include <QThread>

/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função TERMINATE.
 *
*/
class Trem: public QThread {
    Q_OBJECT
    public:
        /**
         * @brief Constrói um objeto trem
         * @param ID - ID do trem
         * @param x - cordenada do eixo X onde o trem inicia
         * @param y - cordenada do eixo Y onde o trem inicia
         */
        Trem(int ID,int x,int y);

        /**
         * @brief Atualizar a velocidade do trem
         * @param velocidade - Nova velocidade a ser inserida no trem
        */
        void set_velocidade(int velocidade);


        /**
         * @brief Função a ser executada após executar trem->start().
         */
        void run();

    //Cria um sinal
    signals:
        void updateGUI(int,int,int);

    private:
        int x;      //posição X do trem na tela | posicão auxiliar do X.
        int y;      //posição Y do trem na tela | posicão auxiliar do Y.
        int ID;          //ID do trem
        int velocidade;  //Velocidade. É o tempo de dormir em milisegundos entre a mudança de posição do trem
        bool parado;      // Armazena se o trem está parado ou não. True para 'parado' e False para 'em movimento'.

        /**
         * @brief Movimenta o trem com base no seu ID
         *
         * @param opcoesCoordenadas - Parâmetros das coordenadas que serão usadas para a movimentação
         */
        void move(const int opcoesCoordenadas[5][4]);
};

#endif // TREM_H
