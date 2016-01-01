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
// #include "inc/log.h"


AudioThing::AudioThing(QObject *parent)
    : QMediaPlayer(parent)
{
    // QMediaPlayer *audioThing = new QMediaPlayer();
    // audioThing->setMedia(
    //     QUrl::fromLocalFile("test/packs/seasons/spring/Celtic Impulse.mp3"));
    // audioThing->setVolume(50);
    // audioThing->play();
    // QMediaPlayer *audioThing2 = new QMediaPlayer();
    // audioThing2->setMedia(
    //     QUrl::fromLocalFile("test/packs/seasons/SeasonAutumn/SeasonAutumn03.mp3"));
    // audioThing2->setVolume(100);
    // audioThing2->play();
    // LogMonitor *logMon = new LogMonitor();
}

void AudioThing::playSfx()
{
}

void AudioThing::playMusic()
{
}

void AudioThing::parsePattern(QString line)
{
    QRegularExpression *regex = new QRegularExpression("dat pattern doe");
    QRegularExpressionMatch match = regex->match(line);
}
