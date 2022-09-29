//
// Created by Alessandro on 29/09/22.
//
#include "player.cpp"

void Player::move() {
    if(this->pimpl->history ==nullptr){
        throw player_exception{player_exception::index_out_of_bounds, "empty history"};
    }else{
        switch(this->pimpl->player_nr){
            case 1:

                break;
            case 2:
                break;
        }
    }
}