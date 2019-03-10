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


/****************************************************************************
**
** This is a modified version of the Qt ScribbleArea for the program purpose.
**
****************************************************************************/


#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    /**
     * @brief openImage
     * @param fileName of the image with a perfored airdrum
     * @return true if the file is opened correctly, false otherwise
     */
    void openImage(const QString &fileName);

    /**
     * @brief saveImage
     * @param fileName of the processed image to save
     * @param fileFormat of the image
     * @return true if the file is saved correctly, false otherwise
     */
    bool saveImage(const QString &fileName, const char *fileFormat);

    /**
     * @brief setPenColor
     * @param newColor, new color of the pen
     */
    void setPenColor(const QColor &newColor);

    /**
     * @brief setPenWidth
     * @param newWidth, the width of the pen
     */
    void setPenWidth(int newWidth);

    /**
     * @brief isModified
     * @return true if the image is modified, false if not
     */
    bool isModified() const { return modified; }

    /**
     * @brief penColor
     * @return the actual color of the pen
     */
    QColor penColor() const { return myPenColor; }

    /**
     * @brief penWidth
     * @return the actual width of the pen
     */
    int penWidth() const { return myPenWidth; }

signals:
    void saveImage();

public slots:
    void clearImage();
    void resetAirdrum();
    void resetPunction();
    void acceptAirdrum();
    void acceptPunction();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawText(QImage *image);
    void drawLineTo(QImage *image, const QPoint &endPoint, bool fillImage = false);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QImage image_aux;
    QImage image_segmented;
    QImage image_original;
    QImage image_tympanum;
    QPoint lastPoint;
    QPoint firstPoint;

    float percentage;

    QPainterPath path;
};

#endif
