#include "game.h"
tuple<int, int> GamePlay::position() {
    return *_snake->begin();
}

tuple<int, int> GamePlay::direction() {
    return _direction;
}

tuple<int, int> GamePlay::food() {
    return _food;
}

deque<tuple<int, int> >* GamePlay::snake() {
    return _snake;
}

int GamePlay::speed() {
    return _speed;
}

void GamePlay::setPos(tuple<int, int> position) {
    this->_position = position;
}

void GamePlay::setDir(tuple<int, int> direction) {
    this->_direction = direction;
}
GamePlay::GamePlay(int width, int height, int speed){
    srand(time(NULL));

    this->height = height;
    this->width = width;
    this->_speed = speed;

    _position = tuple<int, int>();
    _direction = tuple<int, int>();
    _food = tuple<int, int>();

    _snake = new deque<tuple<int, int>>();
    _snake->push_front(_position);

    makeFood();
}

void GamePlay::tick(){
    _position = tuple<int, int>(get<0>(position()) + get<0>(direction()), get<1>(position()) + get<1>(direction())); // the new position
    correctPosition();

    if (validPosition(_position)) {
        if (_position == _food) {
            makeFood();
            _snake->push_front(_position);
        }
        else {
            _snake->push_front(_position);
            _snake->pop_back();
        }
    }
    else {
        resetGame();
    }
}

void GamePlay::correctPosition(){
    if (get<0>(_position) < -width/2) {
        get<0>(_position) = width/2 - 1;
    }
    else if (get<0>(_position) > width/2 - 1) {
        get<0>(_position) = -width/2;
    }
    else if (get<1>(_position) < -height/2) {
        get<1>(_position) = height/2 - 1;
    }
    else if (get<1>(_position) > height/2 - 1) {
        get<1>(_position) = -height/2;
    }
}

bool GamePlay::validPosition(tuple<int, int> position){
    for (int i = 0; i < _snake->size(); i++) {
        if (get<0>(position) == get<0>(_snake->at(i)) && get<1>(position) == get<1>(_snake->at(i))) {
            return false;
        }
    }
    return true;
}

void GamePlay::resetGame(){
    _snake->clear();
    _snake->push_front(_position);
}

void GamePlay::makeFood(){
    _food = tuple<int, int>((rand()%width - (width/2)), (rand()%height- (height/2)));
    while (!validPosition(_food)) {
        _food = tuple<int, int>((rand()%width - (width/2)), (rand()%height- (height/2)));
    }
}


