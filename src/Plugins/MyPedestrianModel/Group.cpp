//
// Created by florenciaprado on 13/02/19.
//

#include "Group.h"

Group :: Group()
{
    m_num_people = 0;
}

Group :: Group(int num_people)
{
    m_num_people = num_people;
    m_people.resize(num_people);
    m_distance_coeff = m_time_coeff = m_discomfort_coeff = 1.0f;
}

Group :: Group(int num_people, glm::vec3 colour)
{
    m_num_people = num_people;
    m_people.resize(num_people);
    m_distance_coeff = m_time_coeff = m_discomfort_coeff = 1.0f;
    m_colour = colour;
}

void Group :: setupGrid(SharedGrid &shared_grid)
{
    m_grid.copyGridSize(shared_grid);
}

void Group :: setSpeeds(float min, float max)
{
    min_speed = min;
    max_speed = max;
}

void Group :: assignRandomLocs()
{
    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(m_grid.m_width, m_grid.m_height);
}


void Group :: assignRandomLeftLoc()
{
    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(1, 2, 0, m_grid.m_height);
}


void Group :: assignRandomRightLoc()
{
    int width = m_grid.m_width;

    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(width-2, width-1, 0, m_grid.m_height);
}



void Group :: assignRandomBottomRightLoc()
{
    int width = m_grid.m_width;

    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(width-5, width-1, 1, 2);
}
void Group :: assignRandomBottomLeftLoc()
{
    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(1, 5, 1, 2);
}
void Group :: assignRandomTopLeftLoc()
{
    int height = m_grid.m_height;

    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(1, 5, height-2, height-1);
}
void Group :: assignRandomTopRightLoc()
{
    int width = m_grid.m_width;
    int height = m_grid.m_height;

    for(int i=0; i<m_num_people; i++)
        m_people[i].assignRandomLoc(width-5, width-1,  height-2, height-1);
}

#pragma region GOAL SETTING
void Group :: setRightSideGoal()
{
    int height = m_grid.m_height;
    int right = m_grid.m_width - 1;

    m_goal.resize(height);

    for(int i=0; i<height; i++)
        m_goal[i] = glm::vec2(right, i);
}


void Group :: setLeftSideGoal()
{
    int height = m_grid.m_height;
    int left = 0;

    m_goal.resize(height);

    for(int i=0; i<height; i++)
        m_goal[i] = glm::vec2(left, i);
}

void Group :: setBottomRightCornerGoal()
{
    int width = m_grid.m_width-1;
    int bottom = 0;

    int size = 5;
    m_goal.resize(size);

    for(int i = 0; i<size; i++)
        m_goal[i] = glm::vec2(width - i, bottom);
}
void Group :: setBottomLeftCornerGoal()
{
    int bottom = 0;

    int size = 5;
    m_goal.resize(size);

    for(int i = 0; i<size; i++)
        m_goal[i] = glm::vec2(i, bottom);
}
void Group :: setTopRightCornerGoal()
{
    int width = m_grid.m_width-1;
    int top = m_grid.m_height-1;

    int size = 5;
    m_goal.resize(size);

    for(int i = 0; i<size; i++)
        m_goal[i] = glm::vec2(width - i, top);
}
void Group :: setTopLeftCornerGoal()
{
    int top = m_grid.m_height-1;

    int size = 5;
    m_goal.resize(size);

    for(int i = 0; i<size; i++)
        m_goal[i] = glm::vec2(i, top);
}

#pragma endregion


void Group :: move(float dt)
{
    for(int i=0; i<m_num_people; i++)
        m_people[i].move(dt);
}