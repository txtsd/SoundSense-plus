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

#include "inc/log.h"
#include "inc/model.h"

// #include "inc/cp437.h"

// LogMonitor::LogMonitor(QObject *parent)
LogMonitor::LogMonitor()
{
    QString gamelog = "test/test.log";
    watcher = new QFileSystemWatcher(QStringList() << gamelog);
    file = new QFile(gamelog);

    if (!file->open(QIODevice::ReadOnly /*| QIODevice::Text*/)) {
        qDebug() << "QIODevice derped out in log.cpp";
    }

    // QTextStream in(file);
    in = new QTextStream(file);
    // QCodePage437Codec *cp437 = new QCodePage437Codec();
    // in.setCodec(cp437);
    in->setCodec("cp437");
    connect(watcher, &QFileSystemWatcher::fileChanged,
            this, &LogMonitor::readLog);
    // qDebug() << QTextCodec::availableCodecs();
}

// void LogMonitor::sendLogLine(QString line)
// {
//     // return line;
// }

void LogMonitor::readLog()
{
    while (!in->atEnd()) {
        QString line = in->readLine();
        // qDebug() << line;
        // emit(&LogMonitor::sendPattern);
        emit sendLogLine(line);
    }

    emit sendTookThisLong();
}
