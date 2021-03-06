/*
# Cyan <http://prepress.sf.net> <https://cyan.fxarena.net>,
# Copyright (C) 2016, 2017, 2018 Ole-André Rodlie<ole.andre.rodlie@gmail.com>
#
# Cyan is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation.
#
# Cyan is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Cyan.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
*/

#include "imageview.h"
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent)
, fit(false) {
    setAcceptDrops(true);
    setBackgroundBrush(Qt::darkGray);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageView::wheelEvent(QWheelEvent* event) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if (event->delta() > 0) { // up
        fit = false;
        scale(scaleFactor, scaleFactor);
        emit myZoom(scaleFactor, scaleFactor);
    } else { // down
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        emit myZoom(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        fit = false;
        emit resetZoom();
    } else {
        if (event->button() == Qt::RightButton) {
            setFit(true);
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }
}

void ImageView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ImageView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ImageView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void ImageView::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls()) { return; }
    if (mimeData->urls().at(0).isEmpty()) { return; }
    QUrl url = mimeData->urls().at(0);
    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(url.toLocalFile());
    if (type.name().startsWith("image")) { emit openImage(url.toLocalFile()); }
    else if (type.name().contains("iccprofile")) { emit openProfile(url.toLocalFile()); }
}

void ImageView::resizeEvent(QResizeEvent */*event*/)
{
    if (fit) {
        fitInView(0,
                  0,
                  scene()->width(),
                  scene()->height(),
                  Qt::KeepAspectRatio);
    }
}

void ImageView::doZoom(double scaleX, double scaleY)
{
    scale(scaleX,scaleY);
}

void ImageView::setFit(bool value)
{
    fit = value;
    fitInView(0,
              0,
              scene()->width(),
              scene()->height(),
              Qt::KeepAspectRatio);
}
