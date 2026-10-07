/**
 * @file Light.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "sim/entities/EntityBase.h"

/**
 * @brief class representing the lights in the simulation, inherits EntityBase
 */
class Light : public EntityBase {
public:

    /**
     * @brief consturctor for light
     * 
     * @param pos - starting position for light in Vector3 form
     * @param dir - starting direction for light in Vector3 form
     * @param radius - radius of the light for its size
     * @param motion - motion specification for the light in json form
     */
    Light(Vector3 pos, Vector3 dir, double radius, const json& motion);
    
    /**
     * @brief destructor for light
     */
    virtual ~Light() {}
   
    /**
     * @brief update function for light, which moves the light entitiy according to its motion type
     * 
     * @param dt - how much time passed since last update
     */
    void Update(double dt);

private:
    Vector3 vel; // velocity vector
    std::string motion; // string to store motion type
    Vector3 p1; // point 1 for back and forth
    Vector3 p2; // point 2 for back and forth
    Vector3 target; // current target of the back and forth movement
    Vector3 rotation_center; // rotation_center for around movement
    double rotation_radius; // radius for the around movement
    double angle; // angle for the around movement - helper vairable for movement
};

#endif