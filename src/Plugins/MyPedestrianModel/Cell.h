//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_CELL_H
#define FULL_MENGE_PED_SIM_CELL_H

#include <iostream>
#include <math.h>
#include <vector>
//#include <glm/glm.hpp> //TODO: sudo apt-get install libglm-dev
#include "CellFace.h"


class Cell {
    glm::vec2 position;

    glm::vec2 gridSize;

    glm::vec2 getNeighbourPos(CellFace face);

    virtual void setFaces() = 0;

};

class SharedCell : public Cell
{
public:

    float m_density, m_height;
    glm::vec2 m_avg_velocity;
    float m_discomfort;


    SharedCellFace m_faces[4];

    SharedCell();

    void setFaces();
};

class GroupCell : public Cell
{
public:

    float m_potential;
    float m_temp_potential;

    glm::vec2 m_tot_velocity;


    GroupCellFace m_faces[4];

    GroupCell();

    void setFaces();

    GroupCellFace getFaceByNeighbour(glm::vec2 neighbour_pos);

    void unTempPotential();
};

#endif //FULL_MENGE_PED_SIM_CELL_H
