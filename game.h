#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <tuple>
#include <deque>
#include <time.h>

using namespace std;
class GamePlay
{
public:
    GamePlay(int width, int height, int speed);
    tuple<int, int> position();
    tuple<int, int> direction();
    tuple<int, int> food();
    deque<tuple<int, int>>* snake();
    int speed();

    void setPos(tuple<int, int> position);
    void setDir(tuple<int, int> direction);
    void tick();

private:
    int width, height, _speed;
    tuple<int, int> _position;
    tuple<int, int> _food;
    tuple<int, int> _direction;

    deque<tuple<int, int>> *_snake;
    void correctPosition();
    bool validPosition(tuple<int, int> position);
    void resetGame();
    void makeFood();

};

#endif // GAME_H
