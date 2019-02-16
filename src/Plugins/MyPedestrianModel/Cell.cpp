//
// Created by florenciaprado on 12/02/19.
//

#include "Cell.h"
using namespace std;

Cell :: Cell()
{
    cell_position = glm::vec2(0.0f);
    grid_size = glm::vec2(0.0f);
}


SharedCell :: SharedCell() : Cell()
{
    m_density  = 0.0f;
    m_avg_velocity = glm::vec2(0.0f);


}
