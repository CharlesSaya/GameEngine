#include "headers/core/soundEngine.h"

SoundEngine::SoundEngine(){

}


void SoundEngine::ost(){
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile(
                           QFileInfo("../GameEngine/sounds/ost.mp3").absoluteFilePath()));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer * ostPlayer = new QMediaPlayer();
    ostPlayer->setVolume(100);
    ostPlayer->setPlaylist(playlist);
    ostPlayer->play();
}

void SoundEngine::ring(){
    QMediaPlayer * ostPlayer = new QMediaPlayer();
    ostPlayer->setMedia(QUrl::fromLocalFile(
                            QFileInfo("../GameEngine/sounds/ringSong.mp3").absoluteFilePath()));
    ostPlayer->setVolume(50);
    ostPlayer->play();
}

void SoundEngine::grid(){
    QMediaPlayer * ostPlayer = new QMediaPlayer();
    ostPlayer->setMedia(QUrl::fromLocalFile(QFileInfo("../GameEngine/sounds/gridSong.mp3").absoluteFilePath()));
    ostPlayer->setVolume(50);
    ostPlayer->play();
}
