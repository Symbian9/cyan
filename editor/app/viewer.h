/*
#
# Copyright (c) 2018, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
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
#
*/

#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QThread>
#include <QActionGroup>
#include <QMdiArea>
#include <Magick++.h>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QComboBox>
#include <QStatusBar>
#include <QMap>

#include "view.h"
#include "layertree.h"
#include "common.h"

class ImageHandler : public QObject
{
    Q_OBJECT

public:
    explicit ImageHandler(QObject *parent = 0);
    ~ImageHandler();

signals:
    void returnImage(Magick::Image image);
    void returnComp(Magick::Image image);
    void errorMessage(QString message);
    void warningMessage(QString message);
    void statusMessage(QString message);

public slots:
    void requestImage(QString filename);
    void readImage(QString filename);
    void requestComp(Magick::Image canvas,
                     layersMap layers,
                     compMap comps,
                     posMap pos,
                     visibilityMap visibility);
    void compImage(Magick::Image canvas,
                   layersMap layers,
                   compMap comps,
                   posMap pos,
                   visibilityMap visibility);

private:
    QThread t;
};


class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = NULL);
    ~Viewer();

signals:
    void openImage(QString filename);

private:
    QMdiArea *mdi;
    QToolBar *mainToolBar;
    QToolBar *pluginsToolBar;
    QMenuBar *mainMenu;
    QStatusBar *mainStatusBar;

    QAction *openImageAct;
    QAction *saveImageAct;
    QAction *newImageAct;
    QAction *quitAct;

    QMenu *filterMenu;

    Magick::Image imageData;
    ImageHandler *imageBackend;
    LayerTree *layersTree;
    QDockWidget *layersDock;
    QComboBox *layersComp;
    QStatusBar *statBar;

public slots:
    View* getCurrentView();

private slots:
    void setupUI();
    void saveSettings();
    void loadSettings();
    void saveImage(QString filename);
    void loadImage(QString filename);
    void saveImageDialog();
    void loadImageDialog();
    void handleNewImage(Magick::Image image);
    void handleError(QString message);
    void handleWarning(QString message);
    void handleStatus(QString message);
    void addPlugin(QObject *plugin, QString filename);
    void loadPlugins();
    void applyFilter();
    void addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu, const char *member, QActionGroup *actionGroup = 0);
    void newTab(Magick::Image image);
    void newImage();
    void handleLayerCompChanged(QString comp);
    void populateCompBox();
    void handleLayerActivated(QTreeWidgetItem *item, int col);
    void handleLayerActivated(QTreeWidgetItem *item, QTreeWidgetItem *old);
    void handleLayerSelected(int layer);
    void handleViewClosed();
    void handleLayersUpdated();
};

#endif // VIEWER_H
