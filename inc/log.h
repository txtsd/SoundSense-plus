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

#ifndef INC_LOG_H
#define INC_LOG_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>


class LogMonitor : public QObject
{
    Q_OBJECT

  public:
    // LogMonitor(QObject *parent = 0);
    LogMonitor();

  private slots:
    void readLog();

  signals:
    void sendLogLine(QString);
    void sendTookThisLong();

  private:
    QFileSystemWatcher *watcher;
    QFile *file;
    QTextStream *in;
};

#endif  // INC_LOG_H
