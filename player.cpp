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
    bool possible_move (Player::piece field[8][8], int i, int j, int in, int jn);
    void pop();

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

}//TODO: TEST FUNZ

void Player::Impl::destroy(List& x) {
    if (x){
        destroy(x->next);
        delete x;

    }
}//TODO: TEST FUNZ

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

} //OK

Player::~Player() {
    this->pimpl->destroy(this->pimpl->history);
    delete this->pimpl;
} //DISTRUTTORE OK

Player::Player(const Player& x) {
    this-> pimpl =new Impl;
    this->pimpl->player_nr = x.pimpl->player_nr;
    this->pimpl->copy(this->pimpl->history, x.pimpl->history);
} //COPY CONSTRUCTOR OK

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
} // DEFAULT CONSTRUCTOR OK

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

} //TODO: TEST

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

} //OK

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

} //OK

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

}//TODO: TEST

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
} //OK

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
//todo:CHECK ALL ERROR CASE eof,good ecc

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
//todo: CHECK ALL ERROR CASE eof, good ecc

bool Player::Impl::matching_boards(Player::piece last[8][8], Player::piece previous[8][8]) {
   if(this->tail->prev !=nullptr){
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
    }else{

    }


}
//toDO:COMPLETARE
//TODO:completare in caso history contenga meno di 2 boards

bool Player::Impl::possible_move(Player::piece field[8][8], int i, int j, int in, int jn) {
    Player::piece new_field[8][8];
    for(int m = 0; m <8; m++){
        for(int n = 0; n<8; n++){
            new_field[m][n]=field[m][n];
        }
    }

    switch (field[i][j]) {
        case x:
            if(i<=6) {
                if (jn == j + 1) {
                    if (i <= 5 && j <= 5) {//todo:x mangia verso l'alto verso dx
                        if (field[in][jn] == o && field[in + 1][jn + 1] == e) {
                            new_field[i][j] = e;
                            new_field[in][jn] = e;
                            if (in + 1 == 7) {
                                new_field[in + 1][jn + 1] = X;
                                this->append(new_field);
                                return true;
                            } else {
                                new_field[in + 1][jn + 1] = x;
                                this->append(new_field);
                                return true;
                            }
                        }
                    }
                    if ( j<=6 && field[in][jn] == e) { //todo: x muove verso alto verso dx
                        new_field[i][j] = e;
                        if (in == 7) {
                            new_field[in][jn] = X;
                            this->append(new_field);
                            return true;
                        } else {
                            new_field[in][jn] = x;
                            this->append(new_field);
                            return true;
                        }
                    }
                    return false;
                } else {
                    if (jn == j - 1) {
                        if (i >= 5 && j >= 2) {//todo: x mangia verso alto sx
                            if (field[in][jn] == o && field[in + 1][jn - 1] == e) {
                                new_field[i][j] = e;
                                new_field[in][jn] = e;
                                if (in + 1 == 7) {
                                    new_field[in + 1][jn - 1] = X;
                                    this->append(new_field);
                                    return true;
                                } else {
                                    new_field[in + 1][jn - 1] = x;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                        }
                        if (j >= 1 && field[in][jn] == e) {
                            new_field[i][j] = e;
                            if (in == 7) {
                                new_field[in][jn] = X;
                                this->append(new_field);
                                return true;
                            } else {
                                new_field[in][jn] = x;
                                this->append(new_field);
                                return true;
                            }
                        }else{
                            return false;
                        }
                    }else{
                        return false;
                    }
                }
            }else{
                return false;
            }
            break;
        case X:
            if(in == i-1){//todo: direzione GIÙ
                if(jn==j+1){//todo:direzione DX
                    if(i>=1){
                        if(i>=2 && j<=5){//todo: mangia GIU DX
                            if((field[in][jn]==o||field[in][jn]==O)&& field[in-1][jn+1]==e){
                                new_field[i][j]=e;
                                new_field[in][jn]=e;
                                new_field[in-1][jn+1]=X;
                                this->append(new_field);
                                return true;
                            }
                        }
                        if(j<=6 && field[in][jn]==e){//todo: muove GIU DX
                            new_field[i][j]=e;
                            new_field[in][jn]=X;
                            this->append(new_field);
                            return true;
                        }else{
                            return false;
                        } //FATTO

                    }else{//else return false perchè per i== 0 non posso muovere verso giu
                        return false;
                    }

                }else{
                    if(jn==j-1){//todo: direzione SX
                        if(i>=1){
                            if(i>=2 && j>=2){//todo:Mangia GIU SX
                                if((field[in][jn]==o||field[in][jn]==O)&& field[in-1][jn-1]==e){
                                    new_field[i][j]=e;
                                    new_field[in][jn]=e;
                                    new_field[in-1][jn-1]=X;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                            if(j>=1&&field[in][jn]==e){//todo:movimento GIU SX
                                new_field[i][j]=e;
                                new_field[in][jn]=X;
                                this->append(new_field);
                                return true;
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }

                    }
                }
            }else{
                if(in == i+1){//todo: direzione verso SU
                    if(jn==j+1){//todo:direzione verso DX
                        if(i<=6){
                            if(i<=5 &&j<=5){//todo: mangia SU DX
                                if((field[in][jn]==o || field[in][jn]==O)&&field[in+1][jn+1]==e){
                                    new_field[i][j]=e;
                                    new_field[in][jn]=e;
                                    new_field[in+1][jn+1]=X;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                            if(j<=6 &&field[in][jn]==e){//todo:muove SU DX
                                new_field[in][jn]=X;
                                new_field[i][j]=e;
                                this->append(new_field);
                                return true;
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }else{
                        if(jn ==j-1){//todo:direzione verso SX
                            if(i<=6){
                                if(i<=5 && j>=2){//todo: mangia SU SX
                                    if((field[in][jn]==O||field[in][jn]==o) && field[in+1][jn-1]==e){
                                        new_field[i][j]=e;
                                        new_field[in][jn]=e;
                                        new_field[in+1][jn-1]=X;
                                        this->append(new_field);
                                        return true;
                                    }
                                }
                                if(j>=1&&field[in][jn]==e){//todo:muove SU SX
                                    new_field[i][j]=e;
                                    new_field[in][jn]=X;
                                    this->append(new_field);
                                    return true;
                                }else{
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }

                }else{
                    return false;
                }
            }
            break;
        case o:
            if(i>=1){//todo:direzione GIU
                if(jn==j+1){//todo: direzione DX
                    if(i>=2 && j<=5){//todo:mangia GIU DX
                        if(field[in][jn]==x && field[in-1][jn+1]==e){
                            new_field[i][j]=e;
                            new_field[in][jn]=e;
                            if(in-1==0){
                                new_field[in-1][jn+1]=O;
                                this->append(new_field);
                                return true;
                            }else{
                                new_field[in-1][jn+1]=o;
                                this->append(new_field);
                                return true;
                            }
                        }
                    }
                    if(j>=6 && field[in][jn]==e){//todo: muove GIU DX
                        new_field[i][j]=e;
                        if(in==0){
                            new_field[in][jn]=O;
                            this->append(new_field);
                            return true;
                        }else{
                            new_field[in][jn]=o;
                            this->append(new_field);
                            return true;
                        }
                    }else{
                        return false;
                    }
                }else{
                    if(jn==j-1){//todo:direzione SX
                        if(i>=2&&j>=2){//todo: mangia GIU SX
                            if(field[in][jn]==x && field[in-1][jn-1]==e){
                                new_field[i][j]=e;
                                new_field[in][jn]=e;
                                if(in-1==0){
                                    new_field[in-1][jn-1]=O;
                                    this->append(new_field);
                                    return true;
                                }else{
                                    new_field[in-1][jn-1]=o;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                        }
                        if(j>=1&&field[in][jn]==e){//todo muove GIU SX
                            new_field[i][j]=e;
                            if(in==0){
                                new_field[in][jn]=O;
                                this->append(new_field);
                                return true;
                            }else{
                                new_field[in][jn]=o;
                                this->append(new_field);
                                return true;
                            }
                        }else{
                            return false;
                        }

                    }else{
                        return false;
                    }
                }

            }else{
                return false;
            }
            break;
        case O:
            if(in==i+1){//todo:direzione SU
                if(jn==j+1){//todo:direzione DX
                    if(i<=6){
                        if(i<=5 && j<=5){//todo: mangia SU DX
                            if((field[in][jn]==x||field[in][jn]==X) && field[in+1][jn+1]==e){
                                new_field[i][j]=e;
                                new_field[in][jn]=e;
                                new_field[in+1][jn+1]=O;
                                this->append(new_field);
                                return true;
                            }
                        }
                        if(j<=6 && field[in][jn]==e){//todo: muove SU DX
                            new_field[i][j]=e;
                            new_field[in][jn]=O;
                            this->append(new_field);
                            return true;
                        }else{
                            return false;
                        }
                    }else{return false;}

                }else{
                    if(jn==j-1){//todo: direzione SX
                        if(i<=6){
                            if(i<=5&& j>=2){
                                if((field[in][jn]==X||field[in][jn]==x) && field[in+1][jn-1]==e){
                                    new_field[i][j]=e;
                                    new_field[in][jn]=e;
                                    new_field[in+1][jn-1]=O;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                            if(j>=1 && field[in][jn]==e){
                                new_field[i][j]=e;
                                new_field[in][jn]=O;
                                this->append(new_field);
                                return true;
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }

                    }else{
                        return false;
                    }
                }

            }else{
                if(in==i-1){//todo: direzione GIU
                    if(jn == j+1){//todo: direzione DX
                        if(i>=1){
                            if(i>=2 && j<=5){//todo:mangia GIU DX
                                if((field[in][jn]==x||field[in][jn]==X) && field[in-1][jn+1]==e){
                                    new_field[i][j]=e;
                                    new_field[in][jn]=e;
                                    new_field[in-1][jn+1]=O;
                                    this->append(new_field);
                                    return true;
                                }
                            }
                            if(j<=6 && field[in][jn]==e){//todo:muove GIU DX
                                new_field[i][j]=e;
                                new_field[in][jn]=O;
                                this->append(new_field);
                                return true;
                            }else{
                                return false;
                            }
                        }else{
                            return false;
                        }
                    }else{
                        if(jn == j-1){
                            if(i>=1){
                                if(i>=2 && j>=2){
                                    if((field[in][jn]==X || field[in][jn]==x) && field[in-1][jn-1]==e){
                                        new_field[i][j]=e;
                                        new_field[in][jn]=e;
                                        new_field[in-1][jn-1]=O;
                                        this->append(new_field);
                                        return true;
                                    }
                                }
                                if(j>=1 && field[in][jn]==e){
                                    new_field[i][j]=e;
                                    new_field[in][jn]=O;
                                    this->append(new_field);
                                    return true;
                                }else{
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }else{return false;}
                    }
                }else{
                    return false;
                }
            }
            break;
    }
    return false;
}//todo: TEST

void Player::move(){
    bool moved = false;
    if(this->pimpl->history == nullptr){

        throw player_exception{player_exception::index_out_of_bounds, "empty history"};

    } else{
        if(pimpl->player_nr ==1){
            int i = 0;
            while(i <8 && !moved){
                int j= 0;
                while(j<8 && !moved){
                    if(pimpl->tail->board[i][j]==x){
                        if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board, i, j, i + 1, j + 1)){//in possible move controllare solo se mangia per j+2
                            moved = true;
                        }else{
                            if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board, i, j, i + 1, j - 1)){
                                moved = true;
                            }
                        }
                    }else{
                        if(pimpl->tail->board[i][j] == X){
                            if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j,i-1,j+1)){
                                moved=true;
                            }else{
                                if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j,i-1,j-1 )){
                                    moved =true;
                                }else{
                                    if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j,i+1,j+1)){
                                        moved = true;
                                    }else{
                                        if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j,i+1,j-1) ){
                                            moved = true;
                                        }
                                    }
                                }
                            }


                        }//todo: DEVO COMPLETARE CON  EXCEPTION SE PEDINA NON è NE X ne x??
                    }
                 j++;
                }
                i++;
            }
        }else{
            if(pimpl->player_nr ==2){
                int i = 0;
                while(i<8 && !moved){
                    int j=0;
                    while(j<8 && !moved){
                        if(pimpl->tail->board[i][j]==o){
                            if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j+1)){
                                moved = true;
                            }else{
                                if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j-1)){
                                    moved = true;
                                }
                            }

                        }else{
                            if(pimpl->tail->board[i][j] == O){
                                if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i+1,j+1)){
                                    moved=true;
                                }else{
                                    if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i+1,j-1)){
                                        moved = true;
                                    }else{
                                        if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j+1)){
                                            moved = true;
                                        }else{
                                            if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j-1)){
                                                moved = true;
                                            }
                                        }
                                    }
                                }

                            }
                        }
                        j++;
                    }
                    i++;
                }
            }
        }
        if(!moved){
            Player::piece same_b[8][8];
            for(int m = 0; m<8; m++){
                for(int n = 0; n<8; n++){
                    same_b[m][n]=pimpl->tail->board[m][n];
                }
            }
            this->pimpl->append(same_b);
        }
    }

}//todo: TEST

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
} //OK

bool Player::valid_move() const {
    Player::piece prev_b[8][8];
    Player::piece last_b[8][8];
    if(this->pimpl->tail->prev == nullptr){//una sola board in history
        throw player_exception{player_exception::index_out_of_bounds,"no moves completed, only one board in history"};
    }else{
        for (int r = 0; r<8; r++){
            for(int c = 0; c<8; c++){
                prev_b[r][c] = pimpl->tail->prev->board[r][c];
                last_b[r][c] = pimpl->tail->board[r][c]; //salvo perchè rimuovo dalla history per rieseguire mossa su prev
            }
        }
        if(pimpl->matching_boards(pimpl->tail->board,pimpl->tail->prev->board)){//ultime due board uguali. mossa non valida
            return false;
        }else{

            this->pimpl->pop();
            bool moved =false;

            if(pimpl->player_nr ==1){
                int i = 0;
                while(i <8 && !moved){
                    int j= 0;
                    while(j<8 && !moved){
                        if(pimpl->tail->board[i][j]==x){
                            if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board, i, j, i + 1, j + 1)){//in possible move controllare solo se mangia per j+2
                                moved = true;
                            }else{
                                if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board, i, j, i + 1, j - 1)){
                                    moved = true;
                                }
                            }
                        }else{
                            if(pimpl->tail->board[i][j] == X){
                                if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j,i-1,j+1)){
                                    moved=true;
                                }else{
                                    if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j,i-1,j-1 )){
                                        moved =true;
                                    }else{
                                        if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j,i+1,j+1)){
                                            moved = true;
                                        }else{
                                            if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j,i+1,j-1) ){
                                                moved = true;
                                            }
                                        }
                                    }
                                }


                            }//todo: DEVO COMPLETARE CON  EXCEPTION SE PEDINA NON è NE X ne x??
                        }
                        j++;
                    }
                    i++;
                }
            }else{
                if(pimpl->player_nr ==2){
                    int i = 0;
                    while(i<8 && !moved){
                        int j=0;
                        while(j<8 && !moved){
                            if(pimpl->tail->board[i][j]==o){
                                if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j+1)){
                                    moved = true;
                                }else{
                                    if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j-1)){
                                        moved = true;
                                    }
                                }

                            }else{
                                if(pimpl->tail->board[i][j] == O){
                                    if(i<=6 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i+1,j+1)){
                                        moved=true;
                                    }else{
                                        if(i<=6 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i+1,j-1)){
                                            moved = true;
                                        }else{
                                            if(i>=1 && j<=6 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j+1)){
                                                moved = true;
                                            }else{
                                                if(i>=1 && j>=1 && pimpl->possible_move(pimpl->tail->board,i,j, i-1,j-1)){
                                                    moved = true;
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                            j++;
                        }
                        i++;
                    }
                }
            }
            if(!moved){
                Player::piece same_b[8][8];
                for(int m = 0; m<8; m++){
                    for(int n = 0; n<8; n++){
                        same_b[m][n]=pimpl->tail->board[m][n];
                    }
                }
                this->pimpl->append(same_b);
            }
        }
        bool matching = this->pimpl->matching_boards(pimpl->tail->board, last_b);
        if(matching){//se last_b e pimpl->tail matchano la mossa è valida
            return true;
        }else{ //se la tail != da last_b rimuovo tail dell'ultima mossa e rimetto last_b
            this->pimpl->pop();
            this->pimpl->append(last_b);
            return false;
        }
    }
} //TODO:TEST

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

}*///todo: COMPLETARE, DEVE RICONOSCERE TUTTE LE MOSSE

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

void Player::Impl::pop() {//todo: CONTROLLARE SE E' GIUSTA
    if(this->history ==nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "can't pop from empty history"};
    }else{
        List pc =this->tail;
        if(this->tail->prev ==nullptr){
            this->tail =nullptr;
            this->history = nullptr;
        }else{
            this->tail = this->tail->prev;
            this->tail->next = nullptr;
        }
        delete pc;
    }
}

/*bool Player::wins(int player_nr) const {
    bool win = true;
    if(this->pimpl->matching_boards(this->pimpl->tail->board, this->pimpl->tail->prev->board)){
        win = false;
    }
    return win;
} //todo:COMPLETARE

bool Player::wins() const {

}//TODO:COMPLETARE

bool Player::loses(int player_nr) const {
    if(this->pimpl->matching_boards(pimpl->tail->board, pimpl->tail->prev->board)){
        return true;
    }else{
        return false;
    }
}//TODO:COMPLETARE

bool Player::loses() const {

}*///TODO:COMPLEATARE

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