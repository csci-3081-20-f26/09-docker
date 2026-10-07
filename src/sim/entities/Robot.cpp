/**
 * @file Robot.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */


#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "sim/entities/Robot.h"

Robot::Robot(Vector3 pos, Vector3 dir, double radius,
             double arenaWidth, double arenaHeight, const json& motion) {
  SetType<Robot>("Robot");
  this->pos = pos;
  this->radius = radius;
  if (motion.contains("speed")) {
    this->speed = motion["speed"];
  }
  if (motion.contains("keys")) {
    this->SetAllowedKeys(motion["keys"]);
  }
  vel = dir * speed;
  this->dir = dir;
  this->arenaWidth = arenaWidth;
  this->arenaHeight = arenaHeight;
  collisionMode = false;
  collisionTime = 0.0;
  this->motion = motion["type"];
}

void Robot::SetAllowedKeys(const json& keys) {
    allowed_keys.clear();
    for (std::string key : keys) {
        allowed_keys.push_back(key);
    }
}



// Handles key down events
void Robot::OnKeyDown(std::string key) {
    if (this->allowed_keys.size() < 4) {
        return;
    }
    if (key == this->allowed_keys[0]) up = true;
    if (key == this->allowed_keys[1]) right = true;
    if (key == this->allowed_keys[2]) down = true;
    if (key == this->allowed_keys[3]) left = true;
}

// Handles key up events
void Robot::OnKeyUp(std::string key) {
    if (this->allowed_keys.size() < 4) {
        return;
    }
    if (key == this->allowed_keys[0]) up = false;
    if (key == this->allowed_keys[1]) right = false;
    if (key == this->allowed_keys[2]) down = false;
    if (key == this->allowed_keys[3]) left = false;
    if (key == " ") {
        up = false;
        down = false;
        left = false;
        right = false;
    }
}

void Robot::Update(double dt) {
    if (this->motion == "keyboard") {
        Vector3 move(0.0, 0.0, 0.0);
        vel = Vector3(0.0, 0.0, 0.0);

        if (down) move[1] -= 1.0;
        if (up) move[1] += 1.0;
        if (left) move[0] -= 1.0;
        if (right) move[0] += 1.0;

        if (move.Length() == 0.0) {
            vel = Vector3(0.0, 0.0, 0.0);
            return;
        }

        move = move.Normalized();

        dir = move;
        vel = move * speed;
        pos = pos + vel * dt;

        if (pos[0] > arenaWidth - radius) pos[0] = arenaWidth - radius;
        if (pos[1] > arenaHeight - radius) pos[1] = arenaHeight - radius;
        if (pos[0] < radius) pos[0] = radius;
        if (pos[1] < radius) pos[1] = radius;
        return;
    }

    if (this->motion == "simple") {
        collisionTime += dt;

        if (vel.Length() == 0.0) {
            vel = dir*speed;
        }
        pos = pos + vel*dt;

        if (pos[0] > arenaWidth - radius || pos[1] > arenaHeight - radius ||
            pos[0] < radius || pos[1] < radius) {
        collisionMode = true;
        collisionTime = 0.0;
        vel = vel * -1.0;
        pos = pos + vel * dt;
        }

        if (collisionMode && collisionTime > 2.0) {
        collisionMode = false;
        double angle = M_PI / 4.0;
        double vx = vel[0] * std::cos(angle) - vel[1] * std::sin(angle);
        double vy = vel[0] * std::sin(angle) + vel[1] * std::cos(angle);
        vel[0] = vx;
        vel[1] = vy;
        vel = vel.Normalized() * speed;
        }

        dir = vel.Normalized();
        speed = vel.Length(); 
    }
}
