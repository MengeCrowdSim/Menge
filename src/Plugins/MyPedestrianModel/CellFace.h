//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_CELLFACE_H
#define FULL_MENGE_PED_SIM_CELLFACE_H

class CellFace
{
public:

    float m_angle;
    glm::vec2 m_offset;

    CellFace();

    void setAngle(float angle, glm::vec2 offset);

};

class SharedCellFace : public CellFace
{
public:

    float m_grad_height;

    SharedCellFace();
};

#endif //FULL_MENGE_PED_SIM_CELLFACE_H
