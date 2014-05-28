#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this,"Abrir Imagen",getenv("HOMEPATH"), tr("Imagenes (*.png *.jpg *.bmp)"));
    QFile file(filename);
    if(file.exists()) {
        ui->widget->openImage(filename);
        ui->widget->setPenColor(QColor::fromRgb(255,0,0));
        ui->widget->setPenWidth(1);
    } else {
        QMessageBox::warning(this,"¡El archivo no existe!","La ruta que ha proporcionado no corresponde a ningun archivo\n"
                             "Por favor, especifique una nueva ruta",QMessageBox::Ok);
    }
}
