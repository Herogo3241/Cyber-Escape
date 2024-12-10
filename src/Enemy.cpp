#include "Enemy.h"

Enemy::Enemy(Vector2 pos) : ray(pos, 0.f) {
    this->pos = pos;
    this->angle = 0.f;
}






void Enemy::update(float deltaTime) {
    Vector2 mousePos = GetMousePosition();
    this->angle = atan2(mousePos.y - this->pos.y, mousePos.x - this->pos.x) * RAD2DEG;
    
    
    for(float i = this->angle - 30; i < this->angle + 30; i += 0.5f) {
        this->ray.setAngle(i);
        this->ray.draw();
    }
}


void Enemy::draw() {
    DrawCircle(this->pos.x, this->pos.y, 10, RED);  
}




Enemy::~Enemy() {

}