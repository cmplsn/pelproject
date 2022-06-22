#include <iostream>
#include "player.hpp"

int main() {

    Player a (1);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    Player b(a);
    a.pop();
    a.pop();
    cout<<a.recurrence();
    return 0;
}