#include <iostream>
#include "player.hpp"
using namespace std;

int main() {
    Player a(1);
    Player b(2);
    Player c(a);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    b.load_board("../boards/es.txt");
    a.move();
    a.store_board("../boards/amoved.txt");
    b.load_board("../boards/amoved.txt");
    b.move();
    b.store_board("../boards/bmoved.txt");
    a.load_board("../boards/bmoved.txt");
    cout<<boolalpha<<a.valid_move()<<endl;
    a.move();
    a.store_board("../boards/a2moved.txt");
    cout<<boolalpha<<b.valid_move()<<endl;
    b.move();
    b.store_board("../boards/b2moved.txt");
    cout<<a.recurrence()<<endl;
    cout<<b.recurrence()<<endl;
    return 0;
}