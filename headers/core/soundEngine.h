#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QMediaPlayer>

class SoundEngine : public QObject
{
    Q_OBJECT

private:

public:
    SoundEngine();
    void walk();
    void jump();
    void ost();
};

#endif // SOUNDENGINE_H
