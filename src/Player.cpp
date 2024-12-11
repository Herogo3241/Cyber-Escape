#include "Player.h"
#include <raymath.h>


Player::Player(Vector2 pos){
    this->pos = pos;
}


void Player::keyMovements(float deltaTime){
    if(IsKeyDown(KEY_W)){
        this->pos.y -= this->speed * deltaTime;
    }if (IsKeyDown(KEY_S)){
        this->pos.y += this->speed * deltaTime;
    }if(IsKeyDown(KEY_A)){
        this->pos.x -= this->speed * deltaTime;
    }if(IsKeyDown(KEY_D)){
        this->pos.x += this->speed * deltaTime;
    }
    
}

void Player::update(float deltaTime) {
    this->keyMovements(deltaTime);
    this->pos.x = Clamp(this->pos.x, this->radius, GetScreenWidth() - this->radius );
    this->pos.y = Clamp(this->pos.y, this->radius, GetScreenHeight() - this->radius);
}

void Player::draw(){
    DrawCircle(this->pos.x, this->pos.y, this->radius, GREEN);
}

Player::~Player()
{
}