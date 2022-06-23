#include <iostream>
#include "player.hpp"

char enum_to_char2(Player::piece a){
    switch (a) {
        default:
            throw "no matching piece";
        case Player::piece::x:
            return 'x';
        case Player::piece::X:
            return 'X';
        case Player::piece::o:
            return 'o';
        case Player::piece::O:
            return 'O';
        case Player::piece::e:
            return ' ';
    }

};

int main() {

    Player a;
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    a.load_board("../boards/es_1.txt");
   cout<<boolalpha<<a.valid_move();
    return 0;
}