//
// Created by florenciaprado on 12/02/19.
//

#include "CellFace.h"

using namespace std;

CellFace :: CellFace()
{
    m_angle = 0.0f;
}

void CellFace :: setAngle(float angle, glm::vec2 offset)
{
    m_angle = angle;
    m_offset = offset;
}

SharedCellFace :: SharedCellFace() : CellFace()
{
    m_grad_height = 0.0f;
}