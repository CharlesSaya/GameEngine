#include "headers/core/soundEngine.h"

SoundEngine::SoundEngine(){

}

void SoundEngine::ost(){
    QMediaPlayer * ostPlayer = new QMediaPlayer();
    ostPlayer->setMedia(QUrl::fromLocalFile("../GameEngine/sounds/ost.mp3"));
    ostPlayer->setVolume(50);
    ostPlayer->play();
}
