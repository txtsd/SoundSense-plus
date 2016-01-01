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
// #include "inc/rapidjson/document.h"


ConfigModel::ConfigModel(QObject *parent)
    : QStandardItemModel(parent)
{
    Q_INIT_RESOURCE(icons);
    setupConfig();
    LogMonitor *logMon = new LogMonitor();
    // connect(logMon, &LogMonitor::sendLogLine,
    //         this, &ConfigModel::receiveLogLine);
    connect(logMon, &LogMonitor::sendLogLine,
            this, &ConfigModel::doTheLine);
    connect(logMon, &LogMonitor::sendTookThisLong,
            this, &ConfigModel::tookThisLong);
    // connect(this, &ConfigModel::sgn_setupConfig,
    //         this, &ConfigModel::setupConfig);
    // emit sgn_setupConfig();
}

void ConfigModel::setupConfig()
{
    QTime countTime;
    countTime.start();
    emit btnPbToggle();
    clear();
    it = new QDirIterator("test/packs/",
                          QStringList() << "*.json", QDir::Files,
                          QDirIterator::Subdirectories);

    while (it->hasNext()) {
        QString filename = it->next();
        // qDebug() << filename;
        QStandardItem *parentItem = this->invisibleRootItem();

        if (!filename.endsWith("customHost.json")
                && !filename.endsWith("autoUpdater.json")) {
            QFile *jsonFile = new QFile();
            jsonFile->setFileName(filename);
            // qDebug() << filename;

            if (jsonFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString jsonFileData = jsonFile->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFileData.toUtf8());
                QJsonArray sounds = jsonDoc.object().value("sounds").toArray();
                QStandardItem *fileItem = new QStandardItem(filename/*.remove(0, 45)*/);
                fileItem->setIcon(QIcon(":/folder.svg"));
                fileItem->setFlags(Qt::ItemIsEnabled);
                parentItem->appendRow(fileItem);
                parentItem = fileItem;

                foreach (const QJsonValue &value, sounds) {
                    QJsonObject soundObject = value.toObject();
                    QString pattern = soundObject.value("logPattern").toString();
                    QStandardItem *patternItem = new QStandardItem(pattern);
                    patternItem->setIcon(QIcon(":/library_music.svg"));
                    patternItem->setFlags(Qt::ItemIsEnabled);
                    parentItem->appendRow(patternItem);
                    parentItem = patternItem;

                    if (soundObject.contains("soundFile")) {
                        QJsonArray soundFiles = soundObject.value("soundFile").toArray();

                        foreach (const QJsonValue &value2, soundFiles) {
                            QJsonObject soundFileObject = value2.toObject();
                            QString soundFileName = soundFileObject.value("filename").toString();
                            QStandardItem *filenameItem = new QStandardItem(soundFileName);
                            filenameItem->setIcon(QIcon(":/audiotrack.svg"));
                            filenameItem->setFlags(Qt::ItemIsEnabled
                                                   | Qt::ItemNeverHasChildren);
                            parentItem->appendRow(filenameItem);
                        }
                    }

                    parentItem = parentItem->parent();
                }

                parentItem = parentItem->parent();
            }

            else {
                qDebug() << "QIODevice derped out in model.cpp";
                return;
            }

            jsonFile->close();
        }
    }

    // emitPbReloadConfigSignal();
    // FIXME: Can't sort while threaded
    sort(0);
    // emit layoutChanged();
    // createLookups();
    emit btnPbToggle();
    // -- Reduce the load on doTheLine()
    QStandardItem *levelTop = this->invisibleRootItem();
    QStandardItem *levelOne = new QStandardItem();
    QStandardItem *levelTwo = new QStandardItem();
    TOTAL = 0;
    total.start();
    // QRegularExpression *regex1 = new QRegularExpression();
    // QRegularExpression *regex2 = new QRegularExpression();
    // -- End load reduction
    qDebug("Time elapsed: %d ms", countTime.elapsed());
    // qDebug() << "ended | Row Count:" << rowCount();
}

// void ConfigModel::createLookups()
// {
//     qDebug() << "'Bout to start buildin' a lookup!";
//     QStandardItem *levelTop = this->invisibleRootItem();
//     // qDebug() << "Got top level item";
//
//     if (levelTop->hasChildren()) {
//         for (int i = 0; i < levelTop->rowCount(); i++) {
//             QStandardItem *levelOne = levelTop->child(i);
//             // qDebug() << "level one";
//
//             if (levelOne->hasChildren()) {
//                 for (int j = 0; j < levelOne->rowCount(); j++) {
//                     QStandardItem *levelTwo = levelOne->child(j);
//                     // qDebug() << "level two";
//                     // qDebug() << "before inserting";
//                     Pair thePair;
//                     thePair.regEx = QRegularExpression(levelTwo->text());
//                     thePair.mindex = levelTwo->index();
//                     // qDebug() << levelTwo->text()
//                     //          << "\t"
//                     //          << thePair.regEx
//                     //          << "\t"
//                     //          << thePair.mindex;
//                     lookups.insert(levelTwo->text(), thePair);
//                     // qDebug() << "after inserting";
//                     // if (levelTwo->hasChildren()) {
//                     //     for (int k = 0; k < levelTwo->rowCount(); k++) {
//                     //         QStandardItem *levelThree = levelTwo->child(k);
//                     //         qDebug() << "level three";
//                     //         qDebug() << "before inserting";
//                     //         qDebug() << levelTwo->text()
//                     //                  << "\t"
//                     //                  << levelThree->text();
//                     //         lookups.insert(levelTwo->text(), levelThree->text());
//                     //         qDebug() << "after inserting";
//                     //     }
//                     // }
//                 }
//             }
//         }
//     }
//
//     qDebug() << "We done buildin' a lookup!";
// }

void ConfigModel::receiveLogLine(QString line)
{
    qDebug() << "Captain, we're receiving a signal of some sort: " << line;
}


// Unfinished -> candidate for discard
// void ConfigModel::getDocs()
// {
//     it = new QDirIterator("test/packs/",
//                           QStringList() << "*.json", QDir::Files,
//                           QDirIterator::Subdirectories);
//     jsonDoc = new Document;

//     while (it->hasNext()) {
//         QString filename = it->next();
//     }
// }

// #include "inc/re2/re2.h"

void ConfigModel::doTheLine(QString line)
{
    total.restart();
    // QRegularExpression *regex1 = new QRegularExpression();
    // QRegularExpression *regex2 = new QRegularExpression();
    QRegularExpression regex1("x([0-9]+)", QRegularExpression::DontCaptureOption);
    QRegularExpression regex2("^ .*", QRegularExpression::DontCaptureOption);
    // regex1.optimize();
    // regex2.optimize();
    // regex1->setPattern("x([0-9]+)");
    // regex2->setPattern("^\\s(.+)");
    // re2::RE2 re_1("x([0-9]+)");
    // re2::RE2 re_2("^\s(.+)");
    // std::string stdLine = line.toStdString();

    // if (re2::RE2::FullMatch(stdLine , "x([0-9]+)")
    //         | re2::RE2::FullMatch(stdLine , "^\s(.+)")) {
    //     qDebug() << "It works!";
    //     TOTAL += total.elapsed();
    //     total.restart();
    //     return;
    // }

    if ((regex1.match(line).hasMatch())
            | (regex2.match(line).hasMatch())
            | (line == "")) {
        // qDebug() << "Skipped:" << line;
        TOTAL += total.elapsed();
        total.restart();
        return;
    }

    levelTop = this->invisibleRootItem();
    // qDebug() << "Got top level item";

    if (levelTop->hasChildren()) {
        for (int i = 0; i < levelTop->rowCount(); i++) {
            levelOne = levelTop->child(i);
            // qDebug() << "level one";

            if (levelOne->hasChildren()) {
                for (int j = 0; j < levelOne->rowCount(); j++) {
                    levelTwo = levelOne->child(j);
                    // qDebug() << "level two";
                    // qDebug() << "before inserting";
                    // Pair thePair;
                    // thePair.regEx = QRegularExpression(levelTwo->text());
                    // thePair.mindex = levelTwo->index();
                    // qDebug() << levelTwo->text()
                    //          << "\t"
                    //          << thePair.regEx
                    //          << "\t"
                    //          << thePair.mindex;
                    // lookups.insert(levelTwo->text(), thePair);
                    regex1.setPattern(levelTwo->text());
                    regex1.setPatternOptions(QRegularExpression::DontCaptureOption);
                    // regex1.optimize();
                    // qDebug() << "Made the regex pattern";
                    // return;

                    if (regex1.match(line).hasMatch()) {
                        // qDebug() << "Matched" <<  line << "with" << levelTwo->text();
                        TOTAL += total.elapsed();
                        total.restart();
                        return;
                    }

                    // if (re2::RE2::FullMatch(line.toStdString(), levelTwo->text().toStdString())) {
                    //     qDebug() << "Matched" <<  line << "with" << levelTwo->text();
                    //     TOTAL += total.elapsed();
                    //     total.restart();
                    //     return;
                    // }
                }
            }
        }
    }

    // qDebug() << "Did not find:" << line;
    TOTAL += total.elapsed();
    total.restart();
}

void ConfigModel::tookThisLong()
{
    qDebug("Total time elapsed: %d ms", TOTAL);
}
