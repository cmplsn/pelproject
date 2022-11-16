//
// Created by Alessandro on 29/09/22.
//
/*#include "player.cpp"

void Player::move() {

    bool moved ==false;
    if(this->pimpl->history ==nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "empty history"};
    }else{
        Player::piece board[8][8];
        for(int m = 0; m <8; m++){
            for(int n= 0; n<8; n++){
                board[m][n]=pimpl->tail->board[m][n];
            }
        }
        if(pimpl->player_nr ==1){
            int i = 0;
            while(i<8 && ){
                int j = 0;
                while(j<8){
                    if(board[i][j] == x){
                        if(pimpl->try_move(board,i+1,j+1)){
                           moved =true;
                        } //moving UP+1 RIGHT+1
                    }
                }
            }
        }else{
            if(pimpl->player_nr==2){

            }
        }

    }
}

bool Player::Impl::try_move(Player::piece (*source)[8], int updown, int lr) {

}*/