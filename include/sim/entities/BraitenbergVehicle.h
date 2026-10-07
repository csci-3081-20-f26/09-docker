/**
 * @file BraitenbergVehicle.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef BRAITENBERG_VEHICLE_H_
#define BRAITENBERG_VEHICLE_H_

#include <cmath>
#include <limits>
#include "EntityBase.h"
#include "sim/physics/DifferentialDriveKinematics.h"
#include "core/ISimulationModel.h"
#include "sim/entities/Light.h"

/**
 * @brief class representing the Braitenberg Vehicles (BV) in the simulation, inherits EntityBase
 */
class BraitenbergVehicle : public EntityBase {
public:
    /**
     * @brief Constructor that creates a BV.
     * 
     * @param pos Position of the BV
     * @param dir Direction of the BV
     * @param radius Radius of the BV
     * @param motion Motion behavior with attributes.
     * @param arena The simulation model used for getting other entities.
     */
    BraitenbergVehicle(Vector3 pos, Vector3 dir, double radius, const json& motion, const ISimulationModel& arena, double arenaWidth, double arenaHeight);

    /**
     * @brief Destructor for BV
    */
    virtual ~BraitenbergVehicle();

    /**
     * Update method for entity.
     * 
     * @param dt time since last update
     */
    void Update(double dt);

private:

    /**
     * Reads the sensor from the specific light.
     * 
     * @param light the light locaiton
     * @param sensor the sensor location
     */
    double getSensorReading(Vector3 light, const Vector3 sensor);

    /**
     * Updates sensor location using the current direction and an angle from the current direction.
     * 
     * @param sensor the sensor location
     * @param angleDeg the angle from the direction where the sensor is located.
     */
    void UpdateSensor(Vector3& sensor, double angleDeg);

    DifferentialDriveKinematics drive;
    Vector3 left, right;
    std::string motion;
    const ISimulationModel& arena;
    double maxSpeed = 50.0;
    double arenaWidth;
    double arenaHeight;
};


#endif