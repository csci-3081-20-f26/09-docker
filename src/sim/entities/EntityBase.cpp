/**
 * @file EntityBase.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/entities/EntityBase.h"

EntityBase::EntityBase() {
    static int count = 0;
    id = count;
    count++;
    SetType<EntityBase>("Entity");
    radius = 10.0;
    speed = 0.0;
}