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

#include "inc/audio.h"
#include "inc/gui.h"
#include "inc/model.h"


qreal ldpi;
qreal dpr;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    t.start();
    setupUi(this);
    ldpi = qApp->screens().at(0)->logicalDotsPerInch();
    dpr = qApp->screens().at(0)->devicePixelRatio();
    qApp->setFont(QFont("Droid Sans", ((ldpi * dpr / 12) - dpr + dpr)));
    // Menu Icons
    actionLogLocation->setIcon(QIcon::fromTheme("document-open"));
    actionQuit->setIcon(QIcon::fromTheme("application-exit"));
    actionHelp->setIcon(QIcon::fromTheme("help-contents"));
    actionAbout->setIcon(QIcon::fromTheme("help-about"));
    // Sounds Configuration
    configModel = new ConfigModel();
    configModel->moveToThread(&t);
    soundsConfigTreeView->setModel(configModel);
    soundsConfigTreeView->setUniformRowHeights(1);
    soundsConfigTreeView->setHeaderHidden(1);
    pbReloadConfig->setHidden(1);
    // Signals & Slots
    connect(btnReloadConfig, &QPushButton::clicked,
            configModel, &ConfigModel::setupConfig);
    connect(configModel, &ConfigModel::btnPbToggle,
            this, &MainWindow::togglePbReloadConfig);
    // Audio Player
    audioThing = new AudioThing();
    // Tests
}

void MainWindow::on_actionLogLocation_triggered()
{
    QFileDialog::getOpenFileName(this, tr("gamelog.txt location"), ".",
                                 tr("gamelog.txt (gamelog.txt);;"
                                    "Text files (*.txt);;"
                                    "Log files (*.log);;"
                                    "All files (*)"));
}

void MainWindow::on_actionQuit_triggered()
{
    // qApp->quit();
    // QCloseEvent::closeEvent();
    close();
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, tr("How to use"),
                             tr("Herp the derp!"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About SoundSense+"),
                       tr("SoundSense+ v0.01\n"
                          "Based on SoundSense by zwei2stein\n"
                          "<link Material Design Icons> by Google"));
}

void MainWindow::on_soundsConfigTreeView_clicked(const QModelIndex &index)
{
    // QStandardItem *item = configModel->itemFromIndex(index);
    // qDebug() << index;
    // qDebug() << item;
}

void MainWindow::togglePbReloadConfig()
{
    qDebug() << pbReloadConfig->isHidden();
    pbReloadConfig->setVisible(pbReloadConfig->isHidden());
    btnReloadConfig->setVisible(pbReloadConfig->isHidden());
}
