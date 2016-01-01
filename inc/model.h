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

#ifndef INC_MODEL_H
#define INC_MODEL_H

#include <QtGui/QStandardItemModel>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QTime>
#include <QtGui/QStandardItem>

class LogMonitor;


// struct Pair {
//     QRegularExpression regEx;
//     QModelIndex mindex;
// };


class ConfigModel : public QStandardItemModel
{
    Q_OBJECT

  public:
    ConfigModel(QObject *parent = 0);

  public slots:
    void setupConfig();

  private slots:
    void receiveLogLine(QString);
    // void createLookups();
    void doTheLine(QString line);
    void tookThisLong();

  signals:
    void btnPbToggle();
    // void sgn_setupConfig();

  private:
    void getDocs();
    QDirIterator *it;
    LogMonitor *logMon;
    // QPair<QRegularExpression, QModelIndex>
    // QHash<QString, Pair> lookups;
    QTime total;
    QStandardItem *levelTop, *levelOne, *levelTwo;
    QRegularExpression *regex1, *regex2;
    int TOTAL;
    // re2::RE2 *re_1, *re_2;
};

#endif  // INC_MODEL_H
