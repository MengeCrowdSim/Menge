//
// Created by florenciaprado on 12/02/19.
//
#pragma once



#define FULL_MENGE_PED_SIM_GRID_H


#include <vector>
#include "Cell.h"
namespace MyModel {

    class Grid {
    public:

        /*!
         *	@brief
         *	Posible map positions
         *	Example:
         *
         *	-2  -1   0   1
         *	----------------
         * |   |   |   |   | -1
         *	----------------
         * |   |   |   |   | 0
         *	----------------
         * |   |   |   |   | 1
         *	----------------
         * minX = -2, maxX = 1
         * minY = -1, maxX = 1
         */
        int minX,  minY,  maxX,  maxY;
        int numCellsX, numCellsY;

        /*!
         *	@brief		Constructor
         */
        Grid();

        /*!
         *	@brief		Constructor
         */
        Grid(int min_x, int min_y, int max_x, int max_y);

        void setupGridCells(int min_x, int min_y, int max_x, int max_y);

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

    class SharedGrid : public Grid {
    public:

        /*!
        *	@brief		vector of cells
        */
        std::vector<std::vector<SharedCell>> cell_matrix;

        float max_slope, min_slope;

        /*!
         *	@brief		Constructor
         */
        SharedGrid();

        /*!
         *	@brief		Constructor
         */
        SharedGrid(int min_x, int min_y, int max_x, int max_y);

        /*!
        *	@brief		Builds the grid with the correct quantity
         *	            of shared cells, and set in every cell the
         *	            grid size and its current position
        */
        void setupGridCells();

        /*!
        *	@brief		Given a cell coordinates returns the corresponding cell
        */
        SharedCell &findCellByPos(glm::vec2 pos);

    };
}