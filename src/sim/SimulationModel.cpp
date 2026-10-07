/**
 * @file SimulationModel.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/SimulationModel.h"

#include <iostream>

#include "sim/entities/Robot.h"
#include "sim/entities/Light.h"
#include "sim/entities/BraitenbergVehicle.h"

SimulationModel::SimulationModel() {}

SimulationModel::~SimulationModel() {}

json SimulationModel::CreateScene(const json& data) {
  json returnVal = data;

  width = returnVal["width"];
  height = returnVal["height"];
  json& entities = returnVal["entities"];
  for (int i = 0; i < entities.size(); i++) {
    IEntity* entity = CreateEntity(entities[i]);
    int id = -1;
    if (entity) {
      AddEntity(entity);
      if (entities[i]["motion"]["type"] == "keyboard") {
        current_robot_idx = this->entities.size()-1;
      }
      id = entity->GetId();
    }
    entities[i]["id"] = id;
  }

  return returnVal;
}

void SimulationModel::Update(double dt) {
  for (int i = 0; i < GetEntities().size(); i++) {
    IEntity *entity = GetEntities()[i];
    entity->Update(dt);
  }
}

IEntity* SimulationModel::CreateEntity(const json& data) const {
  if (data["type"] == "robot") {
    json pos = data["position"];
    json dir = data["direction"];
    Robot* robot = new Robot(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"], width, height,
                     data["motion"]);
    return robot;
  }
  if (data["type"] == "light") {
    json pos = data["position"];
    json dir = data["direction"];
    return new Light(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"],
                     data["motion"]);
  }

  if (data["type"] == "bv") {
    json pos = data["position"];
    json dir = data["direction"];
    return new BraitenbergVehicle(Vector3(pos[0], pos[1], pos[2]),
                     Vector3(dir[0], dir[1], dir[2]).Normalized(),
                     data["radius"],
                     data["motion"], *this, width, height);
  }

  return nullptr;
}

void SimulationModel::AddEntity(IEntity* entity) { entities.push_back(entity); }

void SimulationModel::HandleEvent(const std::string& event, const json& data) {
  // // Space to switch robot control
  if (entities.size() > 0) {
    if (event == "KeyDown" && entities.size() > 0 && data.contains("key") && data["key"] == " ") {
        current_robot_idx++;
        // Do this to not go OB for the idx
        current_robot_idx = current_robot_idx % entities.size();
    }

    // Wasd to move robot, starting with robot with lowest ID

    if (event == "KeyDown") {
        IEntity* entity = entities[current_robot_idx];
        if (entity->IsType<Robot>()) {
            Robot* robot = entity->AsType<Robot>();
            // Don't want an error to happen
            if (!data.contains("key")) {
              return;
            }
            std::string key = data["key"];
            std::vector<std::string> allowed_keys = robot->GetAllowedKeys();
            
            // Ignores inputs that are not in allowed keys
            if ((std::find(allowed_keys.begin(), allowed_keys.end(), key) != allowed_keys.end()) == false) {
                return;
            }
              
            robot->OnKeyDown(data["key"]);
        }
    } else if (event == "KeyUp") {
        IEntity* entity = entities[current_robot_idx];
        if (entity->IsType<Robot>()) {
            if (!data.contains("key")) {
                return;
            }
            Robot* robot = entity->AsType<Robot>();
            robot->OnKeyUp(data["key"]);
        }
    }
  }
}