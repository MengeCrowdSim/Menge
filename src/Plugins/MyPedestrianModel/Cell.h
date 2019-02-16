//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_CELL_H
#define FULL_MENGE_PED_SIM_CELL_H

#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>

class Cell {
public:
    glm::vec2 cell_position;
    glm::vec2 grid_size;


    Cell();

};

class SharedCell : public Cell
{
public:

    float m_density = 0;
    glm::vec2 m_avg_velocity = glm::vec2(0, 0);


    SharedCell();
};


#endif //FULL_MENGE_PED_SIM_CELL_H
