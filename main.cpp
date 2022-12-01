/*#include <iostream>
#include "player.hpp"
using namespace std;

int main() {
    Player a(1);
    Player b(2);
    //Player c(a);
    a.init_board("../boards/es.txt");
    a.load_board("../boards/es.txt");
    return 0;
}*/
#include "player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

int main(){
    int player = 1;
    Player p1(player + 1);
    Player p2(player);
    int round = 0;
    string board_name =  "../Boards/board_" + std::to_string(round) + ".txt";
    p1.init_board(board_name);
    bool game = true;
    while (game) {
        cout<<"partita "<<round<<endl;
        if (round % 2) {
            p2.load_board(board_name);
            if (round > 2 && !p2.valid_move())
                game = false;
            p2.move();
            board_name = "../Boards/board_" + std::to_string(round + 1) + ".txt";
            p2.store_board(board_name);
            if (p2.wins())
                cout << "win" << endl;
            else if (p2.loses())
                cout << "lose" << endl;
        }
        else {
            p1.load_board(board_name);
            if (round > 2 && !p1.valid_move())
                game = false;
            p1.move();
            board_name = "../Boards/board_" + std::to_string(round + 1) + ".txt";
            p1.store_board(board_name);
            if (p1.wins())
                cout << "win" << endl;
            else if (p1.loses())
                cout << "lose" << endl;
        }
        round++;
    }
    cout << "recurrance 2 last board: " << p2.recurrence() << endl;
    cout << "pop 2 last board" << endl;
    p2.pop();
    cout << "recurrance 2 last board: " << p2.recurrence() << endl;
    cout << "recurrance 1 last board: " << p1.recurrence() << endl;
    cout << "pop 1 last board" << endl;
    p1.pop();
    cout << "recurrance 1 last board: " << p1.recurrence() << endl;
    cout << "piece at [1][2] of the 3 board: " << p1.operator()(1, 2, 3) << endl;
    cout << "piece at [1][2] of the 3 board: " << p2.operator()(1, 2, 3) << endl;
    Player p3 = p1;
    cout << "piece at [3][4] of the 5 board: " << p1.operator()(3, 4, 5) << endl;
    cout << "piece at [3][4] of the 5 board: " << p3.operator()(3, 4, 5) << endl;
    Player p4;
    p4.operator=(p2);
    cout << "piece at [3][4] of the 5 board: " << p2.operator()(3, 4, 5) << endl;
    cout << "piece at [3][4] of the 5 board: " << p4.operator()(3, 4, 5) << endl;
    return 0;
}