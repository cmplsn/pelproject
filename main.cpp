#include <iostream>
#include "player.hpp"
using namespace std;
/*int main(){
    Player a (2);
    a.load_board("../boards/board_54.txt");
    a.load_board("../boards/board_55.txt");
    a.valid_move();
}*/

int main(){
    cout<<"mi sono rotto il cazzo"<<endl;
    Player a(1);
    Player b(2);

    int round = 0;
    string name ="../boards/turno_" + std::to_string(round) +".txt";
    a.init_board(name);
    bool game = true;
    while (game && round<=100){
        cout<<"turno "<<round<<endl;
        if(round %2 == 0){
            a.load_board(name);
            if(round >=2 && !a.valid_move()){
                game = false;
            }
            a.move();
            name ="../boards/turno_"+ std::to_string(round+1)+".txt";
            a.store_board(name);
            if(a.wins()){
                cout<<"a vince"<<endl;
            }else if(a.loses()){
                cout<<"a perde"<<endl;
            }
        }
        else{
            b.load_board(name);
            if(round >=2 && !b.valid_move()){
                game=false;
            }
            b.move();
            name ="../boards/turno_"+ std::to_string(round+1)+".txt";
            b.store_board(name);
            if(b.wins()){
                cout<< "b vince"<<endl;
            }else if(b.loses()){
                cout<<"b perde"<<endl;
            }
        }
        round++;
    }
    return 0;
} //todo:MAIN MIOO

/*int main(){
    int round=0;
    Player p1(1);
    Player::piece prova [8][8];
    for(int i = 0; i<8; i++){
        for(int j =0; j <8; j++){
            prova[i][j]= Player::piece::e;
        }
    }
    //p1.store_board("../boards/provacazzo");
    p1.init_board("../boards/nuova.txt");
    p1.load_board("../boards/nuova.txt");
    p1.move();

    p1.store_board("../boards/nuova_"+std::to_string(round)+".txt");
    round++;
    p1.move();
    p1.store_board("../boards/nuova_"+std::to_string(round)+".txt");
    round++;
    p1.move();
    p1.store_board("../boards/nuova_"+std::to_string(round)+".txt");
    round++;
    p1.store_board("../boards/prova_store.txt",2);

    return 0;
}*/ //todo:MAIN DUE



/*int main() {
    Player a(1);
    Player b(2);

    a.init_board("../boards/es.txt");
    int round = 0;
    bool game = true;
    string board_name = "../boards/mossa_" + std::to_string(round) + ".txt";
    a.init_board(board_name);
    while (game) {
        cout<<"mossa "<<round<<endl;
        if (round % 2 == 0) {
            a.load_board(board_name);
            if (round >= 2 && !a.valid_move()) {
                game = false;
            }
            a.move();
            board_name = "../boards/mossa_" + std::to_string(round + 1) + ".txt";
            a.store_board(board_name);
            if (a.wins())
                cout << "a won" << endl;
            else if (a.loses())
                cout << "a lost" << endl;
        }
        else {
            b.load_board(board_name);
            if (round >= 2 && !b.valid_move())
                game = false;
            b.move();
            board_name = "../boards/mossa_" + std::to_string(round + 1) + ".txt";
            b.store_board(board_name);
            if (b.wins())
                cout << "b won" << endl;
            else if (b.loses())
                cout << "b lost" << endl;
        }
        round++;
    }
    cout<<"ultima board di a: "<<a.recurrence()<<endl;
    a.loses();

cout<<"fine progetto aggiornato AGAIN"<<endl;
    return 0;
}*/ //todo: infinite PLAY NON USARE



/*#include "player.hpp"
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
    while (game && round <= 100) {
        cout<<"partita "<<round<<endl;
        if (round % 2 ==0) {
            p2.load_board(board_name);
            if (round >= 2 && !p2.valid_move())
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
            if (round >= 2 && !p1.valid_move())
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

    Player p3 = p1; //USA IL COPY CONSTRUCTOR

    cout << "piece at [3][4] of the 5 board: " << p1.operator()(3, 4, 5) << endl;
    cout << "piece at [3][4] of the 5 board: " << p3.operator()(3, 4, 5) << endl;

    Player p4;
    p4.operator=(p2);   //usa Player::operator=()

    cout << "piece at [3][4] of the 5 board: " << p2.operator()(3, 4, 5) << endl;
    cout << "piece at [3][4] of the 5 board: " << p4.operator()(3, 4, 5) << endl;
    return 0;
}*/ //todo: MAIN DAVIDE