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

#include "inc/model.h"
#include "inc/log.h"


enum roles { regex = Qt::UserRole + 1,
             channel,
             concurrency,
             delay,
             loop,
             multipleMatch,
             probability,
             runLevel,
             timeout,
             balance,
             volumeAdjustment,
             weight,
             author,
             description,
             license,
             url
           };

ConfigModel::ConfigModel(QObject *parent)
    : QStandardItemModel(parent)
{
    Q_INIT_RESOURCE(icons);
    setupConfig();
    LogMonitor *logMon = new LogMonitor();
    connect(logMon, &LogMonitor::sendLogLine,
            this, &ConfigModel::doTheLine);
    connect(logMon, &LogMonitor::sendTookThisLong,
            this, &ConfigModel::tookThisLong);
}

void ConfigModel::setupConfig()
{
    QTime setupTime;
    setupTime.start();
    emit btnPbToggle();
    clear();
    QString filename, jsonFileData, pattern, soundFileName;
    QStandardItem *parentItem, *fileItem, *patternItem, *filenameItem;
    QFile *jsonFile;
    QJsonDocument jsonDoc;
    QJsonArray sounds, soundFiles;
    QJsonObject soundObject, soundFileObject, attrObject;
    it = new QDirIterator("test/packs/",
                          QStringList() << "*.json", QDir::Files,
                          QDirIterator::Subdirectories);

    while (it->hasNext()) {
        filename = it->next();
        // qDebug() << filename;
        parentItem = this->invisibleRootItem();
        jsonFile = new QFile();
        jsonFile->setFileName(filename);
        // qDebug() << filename;

        if (jsonFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            jsonFileData = jsonFile->readAll();
            jsonDoc = QJsonDocument::fromJson(jsonFileData.toUtf8());

            if (jsonDoc.isObject() && jsonDoc.object().contains("sounds")) {
                sounds = jsonDoc.object().value("sounds").toArray();
                fileItem = new QStandardItem(filename/*.remove(0, 45)*/);
                fileItem->setIcon(QIcon(":/folder.svg"));
                fileItem->setFlags(Qt::ItemIsEnabled);
                parentItem->appendRow(fileItem);
                parentItem = fileItem;

                foreach (const QJsonValue &value, sounds) {
                    soundObject = value.toObject();

                    if (soundObject.contains("logPattern")) {
                        pattern = soundObject.value("logPattern").toString();
                        patternItem = new QStandardItem(pattern);
                        QRegularExpression regex(pattern,
                                                 QRegularExpression::DontCaptureOption
                                                 | QRegularExpression::OptimizeOnFirstUsageOption);
                        regex.optimize();
                        QVariant regexQV(regex);
                        patternItem->setIcon(QIcon(":/library_music.svg"));
                        patternItem->setFlags(Qt::ItemIsEnabled);
                        patternItem->setData(regexQV, roles::regex);

                        if (soundObject.contains("channel")) {
                            QVariant tempQV(soundObject.value("channel").toString());
                            patternItem->setData(tempQV, roles::channel);
                        }

                        if (soundObject.contains("concurrency")) {
                            QVariant tempQV(soundObject.value("concurrency").toInt());
                            patternItem->setData(tempQV, roles::concurrency);
                        }

                        if (soundObject.contains("delay")) {
                            QVariant tempQV(soundObject.value("delay").toInt());
                            patternItem->setData(tempQV, roles::delay);
                        }

                        if (soundObject.contains("loop")) {
                            QVariant tempQV(soundObject.value("loop").toString());
                            patternItem->setData(tempQV, roles::loop);
                        }

                        if (soundObject.contains("multipleMatch")) {
                            QVariant tempQV(soundObject.value("multipleMatch").toBool());
                            patternItem->setData(tempQV, roles::multipleMatch);
                        }

                        if (soundObject.contains("probability")) {
                            QVariant tempQV(soundObject.value("probability").toInt());
                            patternItem->setData(tempQV, roles::probability);
                        }

                        if (soundObject.contains("runLevel")) {
                            QVariant tempQV(soundObject.value("runLevel").toInt());
                            patternItem->setData(tempQV, roles::runLevel);
                        }

                        if (soundObject.contains("timeout")) {
                            QVariant tempQV(soundObject.value("timeout").toInt());
                            patternItem->setData(tempQV, roles::timeout);
                        }

                        parentItem->appendRow(patternItem);
                        parentItem = patternItem;

                        if (soundObject.contains("soundFile")) {
                            soundFiles = soundObject.value("soundFile").toArray();

                            foreach (const QJsonValue &value2, soundFiles) {
                                soundFileObject = value2.toObject();
                                soundFileName = soundFileObject.value("filename").toString();
                                filenameItem = new QStandardItem(soundFileName);
                                filenameItem->setIcon(QIcon(":/audiotrack.svg"));
                                filenameItem->setFlags(Qt::ItemIsEnabled
                                                       | Qt::ItemNeverHasChildren);

                                if (soundFileObject.contains("balance")) {
                                    QVariant tempQV(soundFileObject.value("balance").toString());
                                    filenameItem->setData(tempQV, roles::balance);
                                }

                                if (soundFileObject.contains("volumeAdjustment")) {
                                    QVariant tempQV(soundFileObject.value("volumeAdjustment").toString());
                                    filenameItem->setData(tempQV, roles::volumeAdjustment);
                                }

                                if (soundFileObject.contains("weight")) {
                                    QVariant tempQV(soundFileObject.value("weight").toInt());
                                    filenameItem->setData(tempQV, roles::weight);
                                }

                                if (soundFileObject.contains("attribution")) {
                                    attrObject = soundFileObject.value("attribution").toObject();

                                    if (attrObject.contains("author")) {
                                        QVariant tempQV(attrObject.value("author").toString());
                                        filenameItem->setData(tempQV, roles::author);
                                    }

                                    if (attrObject.contains("description")) {
                                        QVariant tempQV(attrObject.value("description").toString());
                                        filenameItem->setData(tempQV, roles::description);
                                    }

                                    if (attrObject.contains("license")) {
                                        QVariant tempQV(attrObject.value("license").toString());
                                        filenameItem->setData(tempQV, roles::license);
                                    }

                                    if (attrObject.contains("url")) {
                                        QVariant tempQV(attrObject.value("url").toString());
                                        filenameItem->setData(tempQV, roles::url);
                                    }
                                }

                                parentItem->appendRow(filenameItem);
                            }
                        }

                        parentItem = parentItem->parent();
                    }
                }

                parentItem = parentItem->parent();
            }
        }

        else {
            qDebug() << "QIODevice derped out in model.cpp";
            return;
        }

        jsonFile->close();
    }

    // FIXME: Can't sort while threaded
    // sort(0);
    // emit layoutChanged();
    emit btnPbToggle();
    regex1 = new QRegularExpression("x([0-9]+)|^ .*|^$",
                                    QRegularExpression::DontCaptureOption
                                    | QRegularExpression::OptimizeOnFirstUsageOption);
    regex1->optimize();
    levelTop = this->invisibleRootItem();
    levelOne = new QStandardItem();
    levelTwo = new QStandardItem();
    logParseTimeMs = 0;
    logParseTime.start();
    qDebug("Setup time: %d ms", setupTime.elapsed());
    // qDebug() << "ended | Row Count:" << rowCount();
}

void ConfigModel::doTheLine(QString line)
{
    logParseTime.restart();

    if ((regex1->match(line).hasMatch())) {
        // qDebug() << "Skipped:" << line;
        logParseTimeMs += logParseTime.elapsed();
        logParseTime.restart();
        return;
    }

    if (levelTop->hasChildren()) {
        for (int i = 0; i < levelTop->rowCount(); i++) {
            levelOne = levelTop->child(i);
            // qDebug() << "level one";

            if (levelOne->hasChildren()) {
                for (int j = 0; j < levelOne->rowCount(); j++) {
                    levelTwo = levelOne->child(j);
                    regex2 = levelTwo->data(roles::regex).toRegularExpression();

                    if (regex2.match(line).hasMatch()) {
                        // qDebug() << "Matched" <<  line << "with" << levelTwo->text();
                        logParseTimeMs += logParseTime.elapsed();
                        logParseTime.restart();
                        return;
                    }
                }
            }
        }
    }

    // qDebug() << "Did not find:" << line;
    logParseTimeMs += logParseTime.elapsed();
    logParseTime.restart();
}

void ConfigModel::tookThisLong()
{
    qDebug("Parsing time: %d ms", logParseTimeMs);
}
