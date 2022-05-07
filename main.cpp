#include <iostream>
#include "player.hpp"

int main() {
    Player a (1);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.store_board("../boards/es_1.txt",0);
    Player b(2);

    return 0;
}
