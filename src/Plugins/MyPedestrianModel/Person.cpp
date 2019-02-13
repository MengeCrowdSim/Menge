//
// Created by florenciaprado on 12/02/19.
//

#include "Person.h"

using namespace std;

Person :: Person()
{
    m_position = m_velocity = glm::vec2(0.0f);

}


void Person :: assignRandomLoc(int grid_width, int grid_height)
{
    int t = rand();

    float w = (rand()%((grid_width)*1000));
    w /= 1000;
    w -= 0.5f;

    float h = (rand()%((grid_height)*1000));
    h /= 1000;
    h -= 0.5f;


    m_position = glm::vec2(w, h);
}

void Person :: assignRandomLoc(int min_width, int max_width, int min_height, int max_height)
{
    int t = rand();

    float w = (rand()%((max_width - min_width)*1000));
    w /= 1000;
    w -= 0.5f;
    w += min_width;

    float h = (rand()%((max_height - min_height)*1000));
    h /= 1000;
    h -= 0.5f;
    h += min_height;

    m_position = glm::vec2(w, h);
}

void Person :: move(float dt)
{
    m_position += dt * m_velocity;
}