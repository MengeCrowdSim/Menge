//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_POPULATIONMANAGER_H
#define FULL_MENGE_PED_SIM_POPULATIONMANAGER_H


#include "Grid.h"
#include "Group.h"

class PopulationManager {
public:

    int m_num_groups;

    std::vector<Group*> m_groups;

    PopulationManager();

    void addGroup(Group &group);

    void assignRandomLocs();

    void setupGroupGrids(SharedGrid &shared_grid);

    void move(float dt);
};


#endif //FULL_MENGE_PED_SIM_POPULATIONMANAGER_H
