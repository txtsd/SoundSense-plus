/*
 * SoundSense+
 * Copyright (C) 2015  txtsd    <thexerothermicsclerodermoid@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef INC_GUI_H
#define INC_GUI_H

#include "build/ui_GUI.h"
#include <QtCore/QModelIndex>
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QScreen>
#include <QtGui/QStandardItem>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

class AudioThing;
class ConfigModel;


class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);

  public slots:
    void togglePbReloadConfig();

  private slots:
    void on_actionLogLocation_triggered();
    void on_actionQuit_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_soundsConfigTreeView_clicked(const QModelIndex &index);

  private:
    ConfigModel *configModel;
    AudioThing *audioThing;
    QThread t;
};

#endif  // INC_GUI_H
