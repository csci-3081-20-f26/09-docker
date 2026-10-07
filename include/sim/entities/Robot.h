/**
 * @file Robot.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "EntityBase.h"


/**
 * @class Robot
 * @brief A robot entity that can be controlled and moves within an arena.
 * 
 * The Robot class extends EntityBase and represents a controllable robot with
 * movement capabilities. It supports keyboard input for directional control,
 * velocity management, and collision detection within a bounded arena.
 * 
 */
class Robot : public EntityBase {
public:
    

    /**
     * @brief Constructor for the Robot class
     * 
     * @param pos Position of the Robot
     * @param dir Direction of the Robot
     * @param radius Radius of the robot
     * @param arenaWidth With of the area
     * @param arenaHeight Height of the arena
     * @param motion Motion type taken from the .json scene file
     */
    Robot(Vector3 pos, Vector3 dir, double radius,
             double arenaWidth, double arenaHeight, const json& motion);
    
    /**
     * @brief Destructor for the Robot class
     * */         
    virtual ~Robot() {}

    /**
     * @brief Updates the robots position
     * 
     * @param dt Delta time for performing euler integration
     */
    void Update(double dt);

    /**
     * @brief Handles when a key is pressed down
     * 
     * @param key The key that was pressed down on the keyboard
     */
    void OnKeyDown(std::string key);

    /**
     * @brief Handles when a key is let go
     * 
     * @param key The key that was let go on the keyboard
     */
    void OnKeyUp(std::string key);


    /**
     * @brief Sets the velocity of the Robot
     * 
     * @param velocity Input velocity
     */
    void SetVelocity(const Vector3& velocity) { vel = velocity; }

    /**
     * @brief Gets the velocity of the Robot
     * 
     * 
     * @returns The velocity of the Robot
     */
    Vector3 GetVelocity() const { return vel; }

    /**
     * @brief Sets the allowed keys for movement defined in the scene .json
     * 
     * @param keys The list of allowed keys
     */
    void SetAllowedKeys(const json& keys);

private:
    Vector3 vel;
    double arenaWidth;
    double arenaHeight;
    bool collisionMode;
    double collisionTime;
    std::string motion;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

#endif