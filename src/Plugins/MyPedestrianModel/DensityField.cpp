//
// Created by florenciaprado on 12/02/19.
//

#include "DensityField.h"


using namespace std;

namespace MyModel {

    MyModel::DensityField::DensityField() {
        density_exponent = 0.1f;
    }

    void MyModel::DensityField::clearDensitiesAndAVGVelocities() {
        int num_cellsX = m_grid->grid_width;
        int num_cellsY = m_grid->grid_height;
        SharedCell *curr_cell;
        glm::vec2 curr_cell_pos;

        for (int i = 0; i < num_cellsX; i++)
            for (int j = 0; j < num_cellsY; j++) {
                curr_cell_pos = glm::vec2(i, j);
                curr_cell = &m_grid->findCellByPos(curr_cell_pos);
                curr_cell->m_density = 0;
                curr_cell->m_avg_velocity = glm::vec2(0.0f);
            }
    }

    void MyModel::DensityField::assignDensitiesAndVelocities(vector<Agent> agents) {

        float deltaX, deltaY, densityA, densityB, densityC, densityD;
        SharedCell *cellA, *cellB, *cellC, *cellD;
        Agent *curr_person;
        glm::vec2 cellApos, cellBpos, cellCpos, cellDpos, person_pos, curr_person_velocity;

        bool left, bot, top, right;

        for (int j = 0; j < agents.size(); j++) {


            curr_person = &agents[j];
            curr_person_velocity = glm::vec2(curr_person->_vel._x, curr_person->_vel._y);
            person_pos = glm::vec2(curr_person->_pos._x, curr_person->_pos._y);


            cellApos = glm::vec2(floor(person_pos.x), floor(person_pos.y));
            cellBpos = glm::vec2(ceil(person_pos.x), floor(person_pos.y));
            cellCpos = glm::vec2(ceil(person_pos.x), ceil(person_pos.y));
            cellDpos = glm::vec2(floor(person_pos.x), ceil(person_pos.y));

            deltaX = person_pos.x - cellApos.x;
            deltaY = person_pos.y - cellApos.y;

            if (m_grid->checkExists(cellApos)) {
                cellA = &m_grid->findCellByPos(cellApos);
                densityA = pow(min(1 - deltaX, 1 - deltaY), density_exponent);
                cellA->m_density += densityA;
                cellA->m_avg_velocity += densityA * curr_person_velocity;
            }

            if (m_grid->checkExists(cellBpos)) {
                cellB = &m_grid->findCellByPos(cellBpos);
                densityB = pow(min(deltaX, 1 - deltaY), density_exponent);
                cellB->m_density += densityB;
                cellB->m_avg_velocity += densityB * curr_person_velocity;
            }

            if (m_grid->checkExists(cellCpos)) {
                cellC = &m_grid->findCellByPos(cellCpos);
                densityC = pow(min(deltaX, deltaY), density_exponent);
                cellC->m_density += densityC;
                cellC->m_avg_velocity += densityC * curr_person_velocity;

            }

            if (m_grid->checkExists(cellDpos)) {
                cellD = &m_grid->findCellByPos(cellDpos);
                densityD = pow(min(1 - deltaX, deltaY), density_exponent);
                cellD->m_density += densityD;
                cellD->m_avg_velocity += densityD * curr_person_velocity;
            }
        }


        divideVelocities();

    }

    void MyModel::DensityField::divideVelocities() {
        int num_cellsX = m_grid->grid_width;
        int num_cellsY = m_grid->grid_height;
        SharedCell *curr_cell;
        glm::vec2 curr_cell_pos;

        float density;

        for (int i = 0; i < num_cellsX; i++)
            for (int j = 0; j < num_cellsY; j++) {
                curr_cell_pos = glm::vec2(i, j);
                curr_cell = &m_grid->findCellByPos(curr_cell_pos);


                density = curr_cell->m_density;
                if (density != 0.0f)
                    curr_cell->m_avg_velocity /= density;
            }
    }

    void MyModel::DensityField::update(vector<Agent> agents) {
        clearDensitiesAndAVGVelocities();
        assignDensitiesAndVelocities(agents);
        std::printf("Density Field updated\n");
    }


    void MyModel::DensityField::assignMinMax(float min, float max) {
        m_min_density = min;
        m_max_density = max;
    }

}

