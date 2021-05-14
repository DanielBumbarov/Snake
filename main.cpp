#include "game.h"
#include "canvas.h"
#include <deque>

#define SIZE 10
#define SPEED 10

using namespace std;

int main(int argc, char *argv[])
{
    GamePlay game(SIZE, SIZE, SPEED);
    Canvas canvas(&game, SIZE, SIZE);
    canvas.init();

    return 0;
}
