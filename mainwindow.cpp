#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : 
                            QMainWindow(parent), 
                                ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //Cria o trem com seu (ID, posição X, posição Y)
    trem1 = new Trem(1,170,40);
    trem2 = new Trem(2,370,90);
    trem3 = new Trem(3,40,190);
    trem4 = new Trem(4,240,240);
    trem5 = new Trem(5,440,190);

    /*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     * Os 3 parâmetros INT do sinal serão utilizados na função.
     * Trem1 e Trem2 são os objetos que podem chamar o sinal. Se um outro objeto chamar o
     * sinal UPDATEGUI, não haverá execução da função UPDATEINTERFACE
     */
    connect(trem1,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem2,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem3,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem4,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem5,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));

    /*
     * Inicia automáticamente a rotação dos trens nos trilhos.
     */
    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();

}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y) {
    switch(id){
    case 1: //Atualiza a posição do objeto da tela (quadrado) que representa o trem1
        ui->trem1->setGeometry(x,y,21,17);
        break;
    case 2: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->trem2->setGeometry(x,y,21,17);
        break;
    case 3: //Atualiza a posição do objeto da tela (quadrado) que representa o trem3
        ui->trem3->setGeometry(x,y,21,17);
        break;
    case 4: //Atualiza a posição do objeto da tela (quadrado) que representa o trem4
        ui->trem4->setGeometry(x,y,21,17);
        break;
    case 5: //Atualiza a posição do objeto da tela (quadrado) que representa o trem5
        ui->trem5->setGeometry(x,y,21,17);
        break;        
    default:
        break;
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

/*
 * Seta a velocidade de cada trem de acordo com a posição
 * do QSlider. Obs.: a velocidade começa em 100 e varia 
 * entre 0 e 200.
 */

void MainWindow::on_velocimetroT1_valueChanged(int value) {
    trem1->set_velocidade(value);
}

void MainWindow::on_velocimetroT2_valueChanged(int value) {
    trem2->set_velocidade(value);
}

void MainWindow::on_velocimetroT3_valueChanged(int value) {
    trem3->set_velocidade(value);
}

void MainWindow::on_velocimetroT4_valueChanged(int value) {
    trem4->set_velocidade(value);
}

void MainWindow::on_velocimetroT5_valueChanged(int value) {
    trem5->set_velocidade(value);
}