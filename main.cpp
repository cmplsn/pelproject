#include <iostream>
#include "player.hpp"

int main() {

    Player a (1);
    Player b(2);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    cout<<a.recurrence()<<endl;
    return 0;
}
