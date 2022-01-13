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
    QMediaPlayer * ringSong = new QMediaPlayer();
    ringSong->setMedia(QUrl::fromLocalFile(
                            QFileInfo("../GameEngine/sounds/ringSong.mp3").absoluteFilePath()));
    ringSong->setVolume(50);
    ringSong->play();
}

void SoundEngine::grid(){
    QMediaPlayer * gridSong = new QMediaPlayer();
    gridSong->setMedia(QUrl::fromLocalFile(QFileInfo("../GameEngine/sounds/gridSong.wav").absoluteFilePath()));
    gridSong->setVolume(50);
    gridSong->play();
}


void SoundEngine::jump(){
    QMediaPlayer * jumpSong = new QMediaPlayer();
    jumpSong->setMedia(QUrl::fromLocalFile(QFileInfo("../GameEngine/sounds/jumpSong.mp3").absoluteFilePath()));
    jumpSong->setVolume(50);
    jumpSong->play();
}

void SoundEngine::laser(){
    QMediaPlayer * laserSong = new QMediaPlayer();
    laserSong->setMedia(QUrl::fromLocalFile(QFileInfo("../GameEngine/sounds/laserSong.wav").absoluteFilePath()));
    laserSong->setVolume(50);
    laserSong->play();
}
