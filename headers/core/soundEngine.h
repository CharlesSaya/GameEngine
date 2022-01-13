#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QMediaPlayer>
#include <QDir>
#include <QMediaPlaylist>

class SoundEngine : public QObject
{
    Q_OBJECT

private:

public:
    SoundEngine();
    void static walk();
    void static jump();
    void static laser();
    void static ost();
    void static ring();
    void static grid();
};

#endif // SOUNDENGINE_H
