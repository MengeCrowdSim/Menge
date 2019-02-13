//
// Created by florenciaprado on 12/02/19.
//
#pragma once

#ifndef FULL_MENGE_PED_SIM_GRID_H
#define FULL_MENGE_PED_SIM_GRID_H


#include <vector>
#include "Cell.h"

class Grid {
public:

    int m_width, m_height;
    /*!
     *	@brief		Constructor
     */
    Grid();

    /*!
     *	@brief		Constructor
     */
    Grid(int width, int height);

    void setupGridCells(int width, int height);

    virtual void setupGridCells() = 0;

    /*!
    *  @brief      Defines if a given position exists in the grid
    */
    bool checkExists(glm::vec2 cell_pos);

    /*!
     *  @brief      Given a pos (x, y) return the closest cell
     */
    glm::vec2 findClosestCellPos(glm::vec2 cell_pos);

    /*!
     *  @brief      Saves the four closest surrounding cells to the
     *              cell passed by parameter in the empty list of cell
     *              passed also by parameter its pointer
     */
    void getNeighbours(glm::vec2 cell, glm::vec2 *cells);

};

class SharedGrid : public Grid
{
public:

    /*!
    *	@brief		vector of cells
    */
    std::vector<std::vector<SharedCell>> m_cells;

    float max_slope, min_slope;
    /*!
     *	@brief		Constructor
     */
    SharedGrid();
    /*!
     *	@brief		Constructor
     */
    SharedGrid(int width, int height);

    /*!
    *	@brief		Builds the grid with the correct quantity
     *	            of shared cells, and set in every cell the
     *	            grid size and its current position
    */
    void setupGridCells();

    /*!
    *	@brief		Given a position returns the corresponding cell
    */
    SharedCell &findCellByPos(glm::vec2 pos);
};

class GroupGrid : public Grid
{

public:
    std::vector<std::vector<GroupCell>> m_cells;


    GroupGrid();
    GroupGrid(int width, int height);

    void setupGridCells();
    GroupCell &findCellByPos(glm::vec2 pos);


    void copyGridSize(SharedGrid &grid);


};



#endif //FULL_MENGE_PED_SIM_GRID_H
