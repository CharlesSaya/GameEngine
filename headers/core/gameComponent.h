#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H


class GameComponent
{
public:

    virtual void input() = 0 ;
    virtual void update() = 0 ;
    virtual void render() = 0 ;
private:

};

#endif // GAMECOMPONENT_H
