//
// Created by florenciaprado on 13/02/19.
//

#ifndef FULL_MENGE_PED_SIM_GROUP_H
#define FULL_MENGE_PED_SIM_GROUP_H


#include "Person.h"
#include "Grid.h"

class Group {
public:

    int m_num_people;

    float m_distance_coeff, m_time_coeff, m_discomfort_coeff;

    float max_speed, min_speed;

    glm::vec3 m_colour;

    std::vector<glm::vec2> m_goal;
    std::vector<Person> m_people;

    GroupGrid m_grid;

    Group();
    Group(int num_people);
    Group(int num_people, glm::vec3 colour);

    void setRightSideGoal();
    void setLeftSideGoal();

    void setBottomRightCornerGoal();
    void setBottomLeftCornerGoal();
    void setTopRightCornerGoal();
    void setTopLeftCornerGoal();

    void setupGrid(SharedGrid &shared_grid);

    void assignRandomLocs();

    void assignRandomLeftLoc();
    void assignRandomRightLoc();

    void assignRandomTopLeftLoc();
    void assignRandomTopRightLoc();
    void assignRandomBottomLeftLoc();
    void assignRandomBottomRightLoc();

    void setSpeeds(float min, float max);

    void move(float dt);

};


#endif //FULL_MENGE_PED_SIM_GROUP_H
