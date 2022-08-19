#include <iostream>
#include "player.hpp"

int main() {
    Player a(1);
    Player b(2);
    Player::piece *board;
    Player::piece other[8][8];
    other[0][0] = *board;
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.store_board("../boards/es_1.txt");
    a.move();
    cout<<"ok"<<endl;
    return 0;
}