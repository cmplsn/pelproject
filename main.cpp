#include <iostream>
#include "player.hpp"
using namespace std;

int main() {
    Player a(1);
    Player b(2);
    Player c(a);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.load_board("../boards/board_1.txt");
    //a.load_board("../boards/es1.txt");
    //a.load_board("../x_mangia.txt");
    cout<<boolalpha<<a.valid_move()<<endl;


    //a.move();

    //cout<<boolalpha<<a.valid_move()<<endl;
    //a.store_board("../boards/ha_mangiato.txt");
    /*a.store_board("../boards/es_1.txt");
    a.move();
    a.move();
    a.move();
    a.move();
    a.move();
    a.store_board("../boards/board_5.txt",0);
    a.store_board("../boards/board_4.txt",1);
    a.store_board("../boards/board_3.txt",2);
    a.store_board("../boards/board_2.txt",3);
    a.store_board("../boards/board_1.txt",4);
    cout<<a(1,0,4)<<endl;*/
    /*cout<<a.recurrence()<<endl;*/
    return 0;
}