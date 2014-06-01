/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include <QtGui>
 #include <QMessageBox>
 #include "mainwindow.h"
 #include "scribblearea.h"
 #include <QFileDialog>


 // -----------------------------------------------------------------------------------------------------
 ScribbleArea::ScribbleArea(QWidget *parent)
     : QWidget(parent)
 {
     setAttribute(Qt::WA_StaticContents);
     modified = false;
     scribbling = false;
     myPenWidth = 1;
     myPenColor = Qt::blue;
 }


 // -----------------------------------------------------------------------------------------------------
 bool ScribbleArea::openImage(const QString &fileName)
 {
     QImage loadedImage;
     if (!loadedImage.load(fileName))
         return false;

//     QSize newSize = loadedImage.size().expandedTo(size());
     //resizeImage(&loadedImage, newSize);

     image_tympanum = image_original = image = loadedImage;

     image_tympanum.fill(qRgb(255, 255, 255));
     image_segmented = image_tympanum;
     modified = false;
     update();
     this->resize(loadedImage.size());

     return true;
 }


 // -----------------------------------------------------------------------------------------------------
 bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
 {
     QImage visibleImage = image;
     resizeImage(&visibleImage, image.size());

     drawText(&visibleImage);

     if (visibleImage.save(fileName, fileFormat)) {
         modified = false;
         return true;
     } else {
         return false;
     }
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::setPenColor(const QColor &newColor)
 {
     myPenColor = newColor;
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::setPenWidth(int newWidth)
 {
     myPenWidth = newWidth;
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::clearImage()
 {
     image.fill(qRgb(255, 255, 255));
     image_original = image_tympanum = image;
     modified = true;
     update();
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::resetAirdrum() {
    image = image_original;
    image_segmented.fill(qRgb(255, 255, 255));
    update();
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::acceptAirdrum() {
     image_aux = image;
     image_tympanum = image_segmented;
     image_segmented.fill(qRgb(255, 255, 255));
     myPenColor = QColor::fromRgbF(0,0,1);
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::resetPunction() {
     image = image_aux;
     image_segmented.fill(qRgb(255, 255, 255));
     update();
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::acceptPunction() {
     int t=0, p=0;
     QSize size = image_tympanum.size();
     QRgb white = qRgb(255,255,255);
     for(int i=0; i< size.width(); i++) {
         for(int j=0; j< size.height(); j++) {
            if(image_tympanum.pixel(i,j) != white) t++;
            if(image_segmented.pixel(i,j) != white) p++;
         }
     }

     percentage = static_cast<float>(p)*100/static_cast<float>(t);

     QMessageBox message(QMessageBox::NoIcon,tr("Resultado"),QString("El tamaño de la"
            "perforación es de un %1 %.\n¿Desea guardar la imagen?").arg(percentage),QMessageBox::Ok|QMessageBox::Cancel,this);

     if( message.exec() ==  QMessageBox::Ok) emit saveImage();
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton) {
         path = QPainterPath(event->pos());
         //path.setFillRule(Qt::WindingFill);
         firstPoint = lastPoint = event->pos();
         scribbling = true;
     }
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
 {
     if ((event->buttons() & Qt::LeftButton) && scribbling) {
         drawLineTo(&image,event->pos());
         drawLineTo(&image_segmented,event->pos(),true);
     }
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton && scribbling) {
         scribbling = false;
         drawLineTo(&image,event->pos());
         drawLineTo(&image_segmented,event->pos(),true);
     }
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);
     QRect dirtyRect = event->rect();
     painter.drawImage(dirtyRect, image, dirtyRect);
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::resizeEvent(QResizeEvent *event)
 {
     if (width() > image.width() || height() > image.height()) {
         int newWidth = qMax(width() + 128, image.width());
         int newHeight = qMax(height() + 128, image.height());
         resizeImage(&image, QSize(newWidth, newHeight));
         update();
     }
     QWidget::resizeEvent(event);
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::drawLineTo(QImage *image, const QPoint &endPoint, bool fillImage)
 {
     QPainter painter(image);
     painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
     if(fillImage) painter.setBrush(QBrush(myPenColor));

     path.lineTo(endPoint);

     painter.drawPath(path);
     if(fillImage) painter.fillPath(path,QBrush(myPenColor));
     //painter.drawLine(lastPoint, endPoint);
     modified = true;

     update();
     lastPoint = endPoint;

     if(!scribbling) {
         path.lineTo(firstPoint);

         painter.drawPath(path.simplified());
         if(fillImage) painter.fillPath(path,QBrush(myPenColor));
         //painter.drawLine(lastPoint, endPoint);
         modified = true;

         update();
     }

 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
 {
     if (image->size() == newSize)
         return;

     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
     QPainter painter(&newImage);
     painter.drawImage(QPoint(0, 0), *image);
     *image = newImage;
 }


 // -----------------------------------------------------------------------------------------------------
 void ScribbleArea::drawText(QImage *image) {
     QPainter painter(image);
     painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
     painter.setBrush(QBrush(myPenColor));

     QPoint point = QPoint(20,20+image->height()*0.05);

     QFont font = painter.font();
     font.setPixelSize(image->height()*0.03);
     painter.setFont(font);
     painter.drawText(point,QString("%1 %").arg(percentage));

     update();
 }
