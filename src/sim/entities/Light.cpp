/**
 * @file Light.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */


#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "sim/entities/Light.h"

Light::Light(Vector3 pos, Vector3 dir, double radius, const json& motion) {
  SetType<Light>("Light");
  this->pos = pos;
  this->radius = radius;
  this->dir = dir;
  this->motion = motion["type"];

  if (motion.contains("speed")) {
    this->speed = motion["speed"];
  }

  if (motion.contains("points")) {
    this->p1 = Vector3(motion["points"][0][0], motion["points"][0][1], motion["points"][0][2]);
    this->p2 = Vector3(motion["points"][1][0], motion["points"][1][1], motion["points"][1][2]);
    this->target = this->p1; // default to start moving towards p1
  }

  if (motion.contains("center")) {
    this->rotation_center = Vector3(motion["center"][0], motion["center"][1], motion["center"][2]);
    Vector3 offset = pos - rotation_center;
    this->rotation_radius = offset.Length();
    this->angle = std::atan2(offset[1], offset[0]);
  }

  vel = dir * speed;
}

void Light::Update(double dt) {

  if (this->motion == "default") return;
  
  if (this->motion == "back_and_forth") {

    Vector3 toTarget = target - pos;
    double distance = toTarget.Length();
    dir = toTarget.Normalized();
    vel = dir * speed;

    // check if we will overshoot on next update
    if (distance < speed * dt) {
      pos = target;
      target = (target[0] == p1[0] && target[1] == p1[1]) ? p2 : p1;
      return;
    }

    // move
    pos = pos + vel * dt;
  }

  if (this->motion == "around") {
    // update angle using angular speed and uses it to reset position
    double omega = speed / rotation_radius;
    angle -= omega * dt;

    // regulate angle variable
    if (angle > M_PI)
      angle -= 2 * M_PI;
    if (angle < -M_PI)
      angle += 2 * M_PI;

    // move
    pos[0] = rotation_center[0] + rotation_radius * std::cos(angle);
    pos[1] = rotation_center[1] + rotation_radius * std::sin(angle);
  }

}
