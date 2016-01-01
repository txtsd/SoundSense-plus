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
    QJsonObject soundObject, soundFileObject;
    it = new QDirIterator("test/packs/",
                          QStringList() << "*.json", QDir::Files,
                          QDirIterator::Subdirectories);

    while (it->hasNext()) {
        filename = it->next();
        // qDebug() << filename;
        parentItem = this->invisibleRootItem();

        if (!filename.endsWith("customHost.json")
                && !filename.endsWith("autoUpdater.json")) {
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
                            patternItem->setData(regexQV);
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
                    regex2 = levelTwo->data().toRegularExpression();

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
