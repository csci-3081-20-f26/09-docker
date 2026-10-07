/**
 * @file BraitenbergVehicle.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */


#include "sim/entities/BraitenbergVehicle.h"

BraitenbergVehicle::BraitenbergVehicle(Vector3 pos, Vector3 dir, double radius, const json& motion, const ISimulationModel& arena,
    double arenaWidth, double arenaHeight) : drive(radius, pos, dir), arena(arena), arenaHeight(arenaHeight), arenaWidth(arenaWidth) {

    SetType<BraitenbergVehicle>("Braitenberg Vehicle");
    this->pos = pos;
    this->dir = dir;
    this->radius = radius;
    UpdateSensor(left, 40.0);
    UpdateSensor(right, -40.0);
    this->motion = motion["type"];
    if (motion.contains("maxSpeed")) {
        this->maxSpeed = motion["maxSpeed"];
    }
}

BraitenbergVehicle::~BraitenbergVehicle() {}

void BraitenbergVehicle::Update(double dt) {
    // find closest light
    Light* closestLight = nullptr;
    double distToLight = std::numeric_limits<double>::max();

    for (IEntity* e : arena.GetEntities()) {
        if (e->IsType<Light>()) {
            Light* light = e->AsType<Light>();
            Vector3 lightPos(light->GetPosition());
            double dist = (lightPos-pos).Length();
            if (dist < distToLight) {
                closestLight = light;
                distToLight = dist;
            }
        }
    }

    double leftVel = 0.0;
    double rightVel = 0.0;

    // Determine wheel speed based on motion behavior and sensor readings.
    if (closestLight) {
        Vector3 lightPos(closestLight->GetPosition());
        if (motion == "explore") {
            leftVel = std::min(1.0/getSensorReading(lightPos, right), maxSpeed);
            rightVel = std::min(1.0/getSensorReading(lightPos, left), maxSpeed);
        }
        else if (motion == "fear") {
            leftVel = std::min(getSensorReading(lightPos, left), maxSpeed);
            rightVel = std::min(getSensorReading(lightPos, right), maxSpeed);
        }
        else if (motion == "agression") {
            leftVel = std::min(getSensorReading(lightPos, right), maxSpeed);
            rightVel = std::min(getSensorReading(lightPos, left), maxSpeed);
        }
        else if (motion == "love") {
            leftVel = std::min(1.0/getSensorReading(lightPos, left), maxSpeed);
            rightVel = std::min(1.0/getSensorReading(lightPos, right), maxSpeed);
        }
    }
    
    // Update position and velocity using differential drive
    Vector3 vel = dir * speed;
    drive.Update(dt, leftVel, rightVel, pos, vel);
    dir = vel.Normalized();
    speed = vel.Length();

    // 
    if (pos[0] < radius || pos[1] < radius || pos[0] + radius > arenaWidth || pos[1] + radius > arenaHeight) {
        dir = dir * -1.0;
        drive.SetDirection(dir);
    }

    // Update sensor locations
    UpdateSensor(left, 40.0);
    UpdateSensor(right, -40.0);
}

double BraitenbergVehicle::getSensorReading(Vector3 light, const Vector3 sensor) {
    return std::max(1800.0/std::pow(1.08, (light - sensor).Length()), 0.0001);
}

void BraitenbergVehicle::UpdateSensor(Vector3& sensor, double angleDeg) {
    double angle = angleDeg*M_PI/180.0;
    Vector3 v = dir;
    double x = v[0] * std::cos(angle) - v[1] * std::sin(angle);
    double y = v[0] * std::sin(angle) + v[1] * std::cos(angle);
    v = Vector3(x, y, 0.0) * radius;
    sensor = pos + v;
}