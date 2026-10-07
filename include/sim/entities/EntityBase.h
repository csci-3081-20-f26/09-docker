/**
 * @file EntityBase.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */
#ifndef ENTITY_BASE_H_
#define ENTITY_BASE_H_

#include "core/IEntity.h"
#include "sim/Vector3.h"

/**
 * @brief EntityBase is a base class that has basic functionality that is shared across entities.  
 */
class EntityBase : public IEntity {
public:

    /** 
     * @brief Default constructor.
     */
    EntityBase();

    /** 
     * @brief Enables polymorphic destructors
     */
    virtual ~EntityBase() {}

    /**
     * @brief Gets the name.
     */
    virtual const std::string& GetName() const { return name; }

    /**
     * @brief Gets the unique entity id.
     */
    virtual int GetId() const { return id; }

    /**
     * @brief Gets the entity's position.
     */
    virtual const double* GetPosition() const { return pos.GetArray(); }

    /**
     * @brief Gets the entity's direction as a unit vector with 3 components.
     */
    virtual const double* GetDirection() const { return dir.GetArray(); }

    /**
     * @brief Gets the entitiy's current speed.
     */
    virtual double GetSpeed() const { return speed; }

    /**
     * @brief Gets the entitiy's radius.
     */
    virtual double GetRadius() const { return radius; }

    /**
     * @brief Updates the entity based on a timestep.
     * @param dt A delta timestep from the last update.
     */
    virtual void Update(double dt) {}

    /**
     * @brief set allowed keys for keyboard controlled robot
     * 
     * @param keys - allowed keys passsed in through json
     */
    virtual void SetAllowedKeys(const json& keys) {};

    /**
     * @brief getter for allowed keys
     *
     */
    virtual const std::vector<std::string> GetAllowedKeys() { return allowed_keys; };

    /**
     * @brief Returns information about the specific C++ type of entity.
     * This is used to efficiently check types rather than dynamic casting.
     */
    const std::type_info& GetType() const { return *type; }

 protected:
  template <typename T>
  void SetType(const std::string& typeName) {
    static const std::type_info& type = typeid(T);
    this->type = &type;
    this->typeName = typeName;
    this->name = typeName + " " + std::to_string(id);
  }

  virtual const std::string& GetTypeName() const { return typeName; }


private:
    int id;
    const std::type_info* type;
    std::string typeName;

protected:
    std::string name;
    std::vector<std::string> allowed_keys;
    Vector3 pos;
    Vector3 dir;
    double speed;
    double radius;
};

#endif