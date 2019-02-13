//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_PERSON_H
#define FULL_MENGE_PED_SIM_PERSON_H

#include <cstdlib>


class Person
{
public:

    glm::vec2 m_position, m_velocity;


    Person();
    void assignRandomLoc(int grid_width, int grid_height);

    void assignRandomLoc(int min_width, int max_width, int min_height, int max_height);

    void move(float dt);
};

#endif //FULL_MENGE_PED_SIM_PERSON_H
