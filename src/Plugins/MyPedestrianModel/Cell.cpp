//
// Created by florenciaprado on 12/02/19.
//

#include "Cell.h"
using namespace std;

Cell :: Cell()
{
    m_position = glm::vec2(0.0f);
}

glm::vec2 Cell :: getNeighbourPos(CellFace face)
{
    return m_position + face.m_offset;
}

SharedCell :: SharedCell() : Cell()
{
    m_density = m_height = m_discomfort = 0.0f;
    m_avg_velocity = glm::vec2(0.0f);


}

void SharedCell :: setFaces()
{
    float curr_angle = 0.0f;
    glm::vec2 offset(1.0f, 0.0f);

    for(int i=0; i<4; i++)
    {
        if(curr_angle == 90)
            offset = glm::vec2(0.0f, 1.0f);
        else if (curr_angle == 180)
            offset = glm::vec2(-1.0f, 0.0f);
        else if (curr_angle == 270)
            offset = glm::vec2(0.0f, -1.0f);

        m_faces[i].setAngle(curr_angle, offset);
        curr_angle += 90.0f;
    }
}