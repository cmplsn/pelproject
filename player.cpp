//
// Created by Alessandro on 05/05/22.
//
#include "player.hpp"

struct Cell{
    Player::piece board[8][8];
    Cell* next;
    Cell* prev;
}; //struct per History/tail in IMPL

typedef Cell* List;

struct Player::Impl{
    int player_nr;
    List history;
    List tail;
    void append(Player::piece y[8][8]);
    void destroy (List x) ;
    List copy(List& dest, List source);

};

List Player::Impl::copy(List &dest, List source) {
    if(source == nullptr){
        return nullptr;//fin qua ok
    }else{
        while(source){
            this->append(source->board);
            source =source->next;
        }
        //append della board;
    }
    return dest;

}//add to copy constructor

void Player::Impl::destroy(List x)  {

} // Function to destroy History. add to Class Destructor


void Player::Impl::append(Player::piece y[8][8]) {
    if(history == nullptr){
        history = new Cell;
        history->prev = nullptr;
        history->next = nullptr;
        for(int i = 0;i<8;i++){
            for(int k= 0; k < 8; k++) {
                history->board[i][k] = y[i][k];
            }
        }
        tail = history;
    }else{
        tail->next = new Cell;
        tail->next->prev = tail;
        tail->next->next =nullptr;

        for(int i = 0;i<8;i++){
            for(int k= 0; k < 8; k++) {
                tail->next->board[i][k] = y[i][k];
            }
        }
        tail = tail->next;
    }

} //FUNZIONA

Player::~Player() {
    //delete this->pimpl->history;
} //todo: delete per History e poi delete pimpl

Player::Player(const Player& x) {
    this-> pimpl =new Impl;
    this->pimpl->player_nr = x.pimpl->player_nr;
    if(x.pimpl->history== nullptr){
        this->pimpl->history = nullptr;
    }else{
        this->pimpl->copy(this->pimpl->history, x.pimpl->history);
        //todo:this->pimpl->history->next = copia del next
        //todo:this->pimpl->history->board = copia enum per enum della board
        //todo:copia della tail
    }


} //todo::COPY CONSTRUCTOR

Player::Player(int player_nr) {
    switch(player_nr){
        default:
            throw player_exception{player_exception::index_out_of_bounds, "select valid player"};
        case 1:
            pimpl=new Impl;
            pimpl->player_nr = 1;
            break;
        case 2:
            pimpl =new Impl;
            pimpl->player_nr = 2;
            break;
    }
}

/*Player& Player::operator=(const Player &) {

}

Player::piece Player::operator()(int r, int c, int history_offset) const {

}*/



char enum_to_char(Player::piece a){
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

Player::piece char_to_enum(char x){
    switch (x) {
        default:
            throw "no matching piece";//throw error
        case 'x':
            return Player::piece::x;
        case 'X':
            return Player::piece::X;
        case 'o':
            return Player::piece::o;
        case'O':
            return Player::piece::O;
        case ' ':
            return Player::piece::e;
    }

}

void Player::init_board(const string &filename) const {
    ofstream myfile(filename);
    piece board[8][8];
    int board_size = sizeof(board)/sizeof(*board);
    int row_size = sizeof(board[0])/sizeof(*board[0]);

    for(int i = 0; i <board_size; i++ ){
        for(int k = 0; k<row_size; k++){
            switch (i) {
                case 0:
                case 2:
                    if (k % 2 == 0) {
                        board[i][k] = e;
                    } else {
                        board[i][k] = x;
                    }
                    break;
                case 1:
                    if (k % 2 == 0) {
                        board[i][k] = x;
                    } else {
                        board[i][k] = e;
                    }
                    break;
                default:
                    board[i][k] = e;
                    break;
                case 5:
                case 7:
                    if (k % 2 == 0) {
                        board[i][k] = o;
                    } else {
                        board[i][k] = e;
                    }
                    break;
                case 6:
                    if (k % 2 == 0) {
                        board[i][k] = e;
                    } else {
                        board[i][k] = o;
                    }
            }

        }
    }

    for(int i = board_size-1; i >= 0; i --){
        for (int k = row_size-1; k >= 0; k--){
            if(k!=0){
                myfile<<enum_to_char(board[i][k])<<' ';
            }else{
                myfile<<enum_to_char(board[i][k]);
            }
        }
        if(i!=0){
            myfile<<endl;}
    }
} // funzionante NO APPEND a History

void Player::store_board(const string &filename, int history_offset) const {
    ofstream myfile(filename);
    int count =0;
    if(!myfile){
        throw player_exception{player_exception::missing_file, "file no found"};
    }else{
        while(pimpl->tail->prev!=nullptr){
            pimpl->tail=pimpl->tail->prev;
            count++;
        }
        if(count!=history_offset){
            throw player_exception{player_exception::index_out_of_bounds, "history shorter than offset requested"};
        }else{
            int boardsize = sizeof(pimpl->tail->board)/ sizeof(*pimpl->tail->board);
            int rowsize=sizeof(pimpl->tail->board[0])/sizeof(*pimpl->tail->board[0]);
            for(int i = boardsize-1; i >= 0; i --){
                for (int k = 0; k < rowsize; k++){
                    if(k!=7){
                        myfile<<enum_to_char(pimpl->tail->board[i][k])<<' ';
                    }else{
                        myfile<<enum_to_char(pimpl->tail->board[i][k]);
                    }
                }
                if(i!=0){
                    myfile<<endl;}
            }
        }
    }
} //da history a file.txt
//todo:controllare tutti casi errore eof,good ecc

void Player::load_board(const string &filename) {
    string name = filename;
    ifstream myfile(filename);
    Player::piece board[8][8];

    if(!myfile){
        throw player_exception{player_exception::missing_file, "file does not exist"};
    }else if (name.substr(name.find_last_of('.')+1)!= "txt") {
        throw player_exception{player_exception::missing_file, "wrong file format"};
    }else{
        while(myfile.good()) {
            for(int i=7; i >=0; i--){
                string line;
                getline(myfile,line);
                if(line.size() != 15){
                    throw player_exception{player_exception::invalid_board, "row size not valid"};
                }else{
                    for(int k=0; k<line.size(); k=k+2){
                        board[i][k/2]= char_to_enum(line.at(k));
                    }

                }
            }

        }
        if(!myfile.eof()){
            throw player_exception{player_exception::invalid_board, "copy incomplete, did not reach end of file"};
        }
    }
    this->pimpl->append(board);

}//da file.txt a history
//todo: check valid board (pedine number, pedine location ecc)
//todo: controllare tutti casi errore file.eof file.good ecc

void Player::move(){

}

bool Player::valid_move() const {

}

void Player::pop() {
    //remove last history board
}

bool Player::wins(int player_nr) const {

}

bool Player::wins() const {

}

bool Player::loses(int player_nr) const {

}

bool Player::loses() const {

}

int Player::recurrence() const {

}
