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
    void destroy (List& x) ;
    List copy(List& dest, List source);
    bool matching_boards(Player::piece last[8][8], Player::piece previous[8][8]);
    bool valid_board(Player::piece y[8][8]);
    void possible_move (Player::piece last_b[8][8], int (&saved_moves)[48]);

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

void Player::Impl::destroy(List& x) {
    if (x){
        destroy(x->next);
        delete x;

    }
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
        tail = tail->next;
        for(int i = 0;i<8;i++){
            for(int k= 0; k < 8; k++) {
                tail->board[i][k] = y[i][k];
            }
        }

    }

} //FUNZIONA

Player::~Player() {
    this->pimpl->destroy(this->pimpl->history);
    delete this->pimpl;
}

Player::Player(const Player& x) {
    this-> pimpl =new Impl;
    this->pimpl->player_nr = x.pimpl->player_nr;
    this->pimpl->copy(this->pimpl->history, x.pimpl->history);


}

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

Player& Player::operator=(const Player &x) {
    if(this != &x){
        if(this->pimpl!= nullptr) {
            pimpl->destroy(pimpl->history);
            pimpl->history = nullptr;
            pimpl->tail = nullptr;
        }
        this->pimpl->player_nr = x.pimpl->player_nr;
        this->pimpl->copy(this->pimpl->history,x.pimpl->history);
    }

return *this;

}

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

}

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

Player::piece Player::operator()(int r, int c, int history_offset) const {
    int cont = 0;
    List pc = pimpl->tail;
    while(pc->prev && cont !=history_offset){
        pc = pc->prev;
        cont++;
    }
    if(cont != history_offset){
        throw player_exception{player_exception::index_out_of_bounds, "history shorter then requested offset"};
    }else{
        if(r<0 ||r >7 || c <0 ||c>7){
            throw player_exception{player_exception::index_out_of_bounds, "requested cell doesn't exist"};
        }else{
            Player::piece x = pc->board[r][c];
            return x;
        }
    }

}//valore di ritorno == numero corrispondente a enum
//FATTO

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
        for (int k = 0; k <row_size; k++){
            if(k!=7){
                myfile<<enum_to_char(board[i][k])<<' ';
            }else{
                myfile<<enum_to_char(board[i][k]);
            }
        }
        if(i!=0){
            myfile<<endl;}
    }
} // funzionante NO APPEND a History
//todo: inserire stringhe direttamente in txt o fare passaggio array -> txt??

void Player::store_board(const string &filename, int history_offset) const {
    ofstream myfile(filename);
    int count =0;
    if(!myfile){
        throw player_exception{player_exception::missing_file, "file no found"};
    }else{
        List pc = pimpl->tail;
        while(pc->prev && count != history_offset){
            pc=pc->prev;
            count++;
        }
        if(count!=history_offset){
            throw player_exception{player_exception::index_out_of_bounds, "history shorter than offset requested"};
        }else{
            int boardsize = sizeof(pc->board)/ sizeof(*pc->board);
            int rowsize=sizeof(pc->board[0])/sizeof(*pc->board[0]);
            for(int i = boardsize-1; i >= 0; i --){
                for (int k = 0; k < rowsize; k++){
                    if(k!=7){
                        myfile<<enum_to_char(pc->board[i][k])<<' ';
                    }else{
                        myfile<<enum_to_char(pc->board[i][k]);
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
    if(pimpl->valid_board(board)) {
        this->pimpl->append(board);
    }else{
        throw player_exception{player_exception::missing_file, "invalid board format"};
    }

}//da file.txt a history
//todo: controllare tutti casi errore file.eof file.good ecc

bool Player::Impl::matching_boards(Player::piece last[8][8], Player::piece previous[8][8]) {
    int count = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j <8; j++){
            if(last[i][j]==previous[i][j]){
                /*cout<<"riga:"<<i<<" colonna:"<<j<<endl;
                 *cout<<"previousboard:"<<enum_to_char(previous[i][j])<<endl;
                 *cout<<"lastcell:"<<enum_to_char(last[i][j])<<endl;*/
                count ++;
            }
        }
    }
    if(count == 64){
        return true;
    }else{
        return false;
    }
}
//todo: completare in caso history contenga meno di 2 boards

void Player::move(){
    int count = 0;
    stack<int> moves;
    stack<int> points;
    piece board[8][8];
    for(int k = 0; k < 8; k++){
        for(int m = 0; m < 8; m++){
            board[k][m] = pimpl->tail->board[k][m];
        }
    }
    if(this->pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "empty history"};
    }else{
        if(pimpl->player_nr==1){
            for(int i = 0; i < 8; i++){
                for(int j = 0; j <8; j++){
                    if(board[i][j]==x){
                        if(i+1 == 7){
                            if(board[i+1][j+1]== e){
                                moves.push(((i+1)*10)+j+1 );
                                points.push(1);
                            }
                        }

                    }
                    if(board[i][j]== X){

                    }
                }
            }

        }else{
            if(pimpl->player_nr ==2){
                for(int i = 0; i < 8; i++){
                    for(int j = 0; j < 8; j++){
                        if(board[i][j]==o){


                        }
                        if(board[i][j]==O){

                        }
                    }
                }
            }
        }
    }

    /*if(this->pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds,"empty history"};
    }else{
        if(pimpl->player_nr == 1){
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if(board[i][j]==x){
                            if(j==0){

                            }
                        }else{
                            if(board[i][j]== o){

                            }
                        }
                        if (i <= 5) {
                            if (j >= 2 && j <= 5) {
                                if (board[i][j] == x) {
                                    if (board[i + 1][j + 1] == e) {
                                        if (board[i + 2][j + 2] == e && board[i + 2][j] == e) {
                                            board[i][j] = e;
                                            board[i + 1][j + 1] = x;
                                        }else{

                                        }

                                    } else {
                                        if (board[i + 1][j - 1] == e) {
                                            if(board[i+2][j-2] == e && board[i+2][j]==e){
                                                board[i][j]=e;
                                                board[i+1][j-1]=x;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
        }
        else{
            if(pimpl->player_nr==2){

            }
        };


    }*/
}//todo: COMPLETARE

bool Player::Impl::valid_board(Player::piece y[8][8]) {
    int count_x = 0;
    int count_e = 0;
    int count_o = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(i%2==0){
                if(j%2==0){
                    if(y[i][j]!= piece::e){
                        return false;
                    }
                }
            }else{
                if(j%2!=0){
                    if(y[i][j]!= piece::e){
                        return false;
                    }
                }
            }

            if(y[i][j]== piece::x|| y[i][j]==piece::X){
                count_x ++;
            }else{
                if(y[i][j]== piece::o ||y[i][j]== piece::O){
                    count_o ++;
                }else{
                    if(y[i][j]== piece::e){
                        count_e ++;
                    }
                }
            }

        }
    }
    if(count_x > 12 || count_o >12){
        return false;
    }
    if(count_e < 40){
       return false;
    }

    int count_tot = count_x + count_e + count_o;
    if (count_tot != 64){
        return false;
    }
    return true;
}

void Player::Impl::possible_move(Player::piece last_b[8][8], int (&saved_moves)[48]) {//passagio array bidimensionale
    if(this->player_nr == 1){
        for(int i = 0; i < 8; i++){
            for(int j = 0; j< 8; j++){
                if(last_b[i][j]==x){//caso player 1 pedina x
                    if(i+1 == 7){
                        if(last_b[i+1][j+1]== e){
                        }
                    }
                }else{
                    if(last_b[i][j]== X){ //caso player 1 DAMA X

                    }
                }
            }
        }
    }else{
        if(this->player_nr == 2){
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    if(last_b[i][j]== o){ //caso player 2 pedina o

                    }else{
                        if(last_b[i][j] == O){//caso player 2 DAMA O

                        }
                    }
                }
            }
        }

    }
}

/*bool Player::valid_move() const {
    Player::piece last [8][8];
    Player::piece previous[8][8];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j<8; j++){
            last[i][j]=pimpl->tail->board[i][j];
            previous[i][j]=pimpl->tail->prev->board[i][j];
        }
    }
    if(this->pimpl->tail->prev == nullptr){//una sola board -> nessuna mossa effettuata
        throw player_exception{player_exception::index_out_of_bounds, "no move possible"};
    }else {
        if (pimpl->matching_boards(pimpl->tail->board, pimpl->tail->prev->board)) {//stessa board ripetuta == nessuna mossa effettuata
            return false;
        }else{
            if(!pimpl->valid_board(pimpl->tail->board)){//numero pezzi, spazi bianchi ecc
                return false;
            }else{
                for(int i = 0; i < 8 ; i++){
                    for(int j = 0; j < 8; j++){
                        if(i== 7 && previous[i][j] == piece::x && last[i][j] !=piece::X){
                            return false;
                        }else{
                            if(i == 0 && previous[i][j]==piece::o && last[i][j]!=piece::O){
                                return false;
                            }
                        }
                        if(previous[i][j] == piece::x ){
                            if(previous[i+1][j+1] == piece::o && previous[i+2][j+2]==piece::e){
                                if(!(last[i][j]==piece::e && last[i+1][j+1]==piece::e && last[i+2][j+2]==piece::x) ){
                                    return false;
                                }
                            }

                        }else{
                            if(last[i][j] == piece::o ){

                            }
                        }
                    }
                }

            }
        }
    }
    return true;

}*/

void Player::pop() {
    if(this->pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "can't pop from empty history"};
    }else{
        List pc = pimpl->tail;


        if(this->pimpl->tail->prev ==nullptr){
            this->pimpl->tail =nullptr;
            this->pimpl->history = nullptr;
        }else{
            this->pimpl->tail = pimpl->tail->prev;
            this->pimpl->tail->next = nullptr;
        }
        delete pc;

    }

}   //FATTO

/*bool Player::wins(int player_nr) const {
    bool win = true;
    if(this->pimpl->matching_boards(this->pimpl->tail->board, this->pimpl->tail->prev->board)){
        win = false;
    }
    return win;
} //todo:COMPLETARE

bool Player::wins() const {

}

bool Player::loses(int player_nr) const {
    if(this->pimpl->matching_boards(pimpl->tail->board, pimpl->tail->prev->board)){
        return true;
    }else{
        return false;
    }
}

bool Player::loses() const {

}*/

int Player::recurrence() const {
    List pc = this->pimpl->tail;

    if(this->pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "history is empty"};
    }else{
        int count = 1 ;
        int correct_char=0;
        while(pc->prev !=nullptr){
            for(int i = 0; i < 8; i ++){
                for (int k = 0; k < 8; k++){

                    if(pc->board[i][k]== pc->prev->board[i][k]){
                        correct_char++;
                    }
                }
            }
            if(correct_char==64){
                count++;
            }
            pc=pc->prev;
        }
        return count;
    }

} //FATTO