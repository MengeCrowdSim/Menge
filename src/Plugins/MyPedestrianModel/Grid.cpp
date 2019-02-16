//
// Created by florenciaprado on 12/02/19.
//

#include "Grid.h"
#include "Cell.h"


using namespace std;

#pragma region BASE

namespace MyModel {
    Grid::Grid() {
        minX = minY = maxX = maxY = 0;
    }


    Grid::Grid(int min_x, int min_y, int max_x, int max_y){
        minX = min_x;
        minY = min_y;
        maxX = max_x;
        maxY = max_y;
        numCellsX = abs(maxX - minX);
        numCellsY = abs(maxY - minY);
    }


    bool Grid::checkExists(glm::vec2 cell_pos) {
        float x = cell_pos.x;
        float y = cell_pos.y;

        if (x <= minX) return false;
        if (x >= maxX) return false;
        if (y <= minY) return false;
        return y < maxY;

    }

    void Grid::getNeighbours(glm::vec2 cell, glm::vec2 *cells) {
        int x = cell.x;
        int y = cell.y;

        cells[0] = glm::vec2(x + 1, y);
        cells[1] = glm::vec2(x, y + 1);
        cells[2] = glm::vec2(x - 1, y);
        cells[3] = glm::vec2(x, y - 1);
    }

    glm::vec2 Grid::findClosestCellPos(glm::vec2 cell_pos) {
        std::vector<glm::vec2> cells_pos(4);

        cells_pos[0] = glm::vec2(glm::floor(cell_pos.x), glm::ceil(cell_pos.y));
        cells_pos[1] = glm::vec2(glm::ceil(cell_pos.x), glm::ceil(cell_pos.y));
        cells_pos[2] = glm::vec2(glm::ceil(cell_pos.x), glm::floor(cell_pos.y));
        cells_pos[3] = glm::vec2(glm::floor(cell_pos.x), glm::floor(cell_pos.y));

        float min_dist = 999;
        float dist;
        glm::vec2 min_cell_pos(-5.0f);

        for (int i = 1; i < 4; i++) {
            if (checkExists(cells_pos[i])) {
                dist = glm::distance(cells_pos[i], cell_pos);
                if (dist < min_dist)
                    min_cell_pos = cells_pos[i];
            }
        }


        return min_cell_pos;
    }

    SharedGrid::SharedGrid() : Grid() {

    }

    SharedGrid::SharedGrid(int min_x, int min_y, int max_x, int max_y) : Grid(min_x, min_y, max_x, max_y) {
        setupGridCells();
    }

    void SharedGrid::setupGridCells() {
        cell_matrix.resize(numCellsX);
        for (int i = 0; i < numCellsX; i++) {
            cell_matrix[i].resize(numCellsY);
            for (int j = 0; j < numCellsY; j++) {
                cell_matrix[i][j].cell_position = glm::vec2(i + minX, j + minY);
                cell_matrix[i][j].grid_size = glm::vec2(numCellsX, numCellsY);
            }
        }
    }

    SharedCell &SharedGrid::findCellByPos(glm::vec2 pos) {
        SharedCell &cell = cell_matrix[pos.x - minX][pos.y - minY];
        return cell;
    }

}
