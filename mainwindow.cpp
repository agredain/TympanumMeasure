#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->cleanImage,SIGNAL(triggered()),ui->widget,SLOT(clearImage()));
    QObject::connect(ui->cleanTympanum,SIGNAL(triggered()),ui->widget,SLOT(resetAirdrum()));
    QObject::connect(ui->cleanPunction,SIGNAL(triggered()),ui->widget,SLOT(resetPunction()));
    QObject::connect(ui->acceptTympanum,SIGNAL(triggered()),ui->widget,SLOT(acceptAirdrum()));
    QObject::connect(ui->acceptPunction,SIGNAL(triggered()),ui->widget,SLOT(acceptPunction()));
    QObject::connect(ui->widget,SIGNAL(saveImage()),this,SLOT(saveFile()));
}

MainWindow::~MainWindow() { delete ui; }


// -----------------------------------------------------------------------------------------------------
void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this,"Abrir Imagen", getenv("HOMEPATH"),
                                                    tr("Imagenes (*.png *.jpg *.bmp)"));
    QFile file(filename);
    if(!file.exists()) {
        QMessageBox::warning(this,tr("¡El archivo no existe!"),tr("La ruta que ha proporcionado no corresponde a ningun archivo\n"
                             "Por favor, especifique una nueva ruta"),QMessageBox::Ok);
    }

    ui->widget->openImage(filename);
    ui->widget->setPenColor(QColor::fromRgb(255,0,0));
    ui->widget->setPenWidth(1);

    ui->cleanImage->setEnabled(true);
    ui->acceptTympanum->setEnabled(true);
    ui->cleanTympanum->setEnabled(true);
}

void MainWindow::acceptAirdrum() {
    ui->acceptPunction->setEnabled(true);
    ui->cleanPunction->setEnabled(true);
}

void MainWindow::acceptPunction() {
    ui->saveImage->setEnabled(true);
    ui->acceptTympanum->setEnabled(false);
    ui->cleanTympanum->setEnabled(false);
}

void MainWindow::saveFile() {
    QString filename = QFileDialog::getSaveFileName(
                this, tr("Abrir Imagen"), getenv("HOMEPATH"), tr("Imagenes (*.jpg)"));
    QFile file(filename);
    if(!file.exists()) {
        ui->widget->saveImage(filename, "jpg");
    } else {
//        QMessageBox::warning(this,tr("¡El archivo no existe!"),tr("La ruta que ha proporcionado no corresponde a ningun archivo\n"
//                             "Por favor, especifique una nueva ruta"),QMessageBox::Ok);
    }
}


// -----------------------------------------------------------------------------------------------------
void MainWindow::cleanImage() {
    ui->cleanImage->setEnabled(false);
    ui->acceptTympanum->setEnabled(false);
    ui->cleanTympanum->setEnabled(false);
    ui->acceptPunction->setEnabled(false);
    ui->cleanPunction->setEnabled(false);
}
