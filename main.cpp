#include <iostream>
#include "player.hpp"

int main() {
    Player a(1);
    Player b(2);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.store_board("../boards/es_1.txt");
    a.move();
    a.move();
    a.move();
    a.store_board("../boards/board_3.txt",0);
    a.store_board("../boards/board_2.txt",1);
    a.store_board("../boards/board_1.txt",2);
    return 0;
}