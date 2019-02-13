//
// Created by florenciaprado on 12/02/19.
//

#include "Grid.h"
#include "Cell.h"


using namespace std;

#pragma region BASE
Grid :: Grid()
{
    m_width = m_height = 0;
}

Grid :: Grid(int width, int height)
{
    m_width = width;
    m_height = height;
}

bool Grid :: checkExists(glm::vec2 cell_pos)
{
    int x = cell_pos.x;
    int y = cell_pos.y;

    if(x < 0)
        return false;
    if(x >= m_width)
        return false;

    if(y < 0)
        return false;
    if(y >= m_height)
        return false;

    return true;
}

void Grid :: getNeighbours(glm::vec2 cell, glm::vec2 *cells)
{
    int x = cell.x;
    int y = cell.y;

    cells[0] = glm::vec2(x+1, y);
    cells[1] = glm::vec2(x, y+1);
    cells[2] = glm::vec2(x-1, y);
    cells[3] = glm::vec2(x, y-1);
}

glm::vec2 Grid :: findClosestCellPos(glm::vec2 cell_pos)
{
    std::vector<glm::vec2> cells_pos(4);

    cells_pos[0] = glm::vec2(glm::floor(cell_pos.x), glm::ceil(cell_pos.y));
    cells_pos[1] = glm::vec2(glm::ceil(cell_pos.x), glm::ceil(cell_pos.y));
    cells_pos[2] = glm::vec2(glm::ceil(cell_pos.x), glm::floor(cell_pos.y));
    cells_pos[3] = glm::vec2(glm::floor(cell_pos.x), glm::floor(cell_pos.y));

    float min_dist = 999;
    float dist;
    glm::vec2 min_cell_pos(-5.0f);

    for(int i=1; i<4; i++)
    {
        if(checkExists(cells_pos[i]))
        {
            dist = glm::distance(cells_pos[i], cell_pos);
            if(dist < min_dist)
                min_cell_pos = cells_pos[i];
        }
    }


    return min_cell_pos;
}

SharedGrid :: SharedGrid() : Grid()
{
    max_slope = min_slope = 0.0f;
}

SharedGrid :: SharedGrid(int width, int height) : Grid(width, height)
{
    min_slope = 0.0f;
    max_slope = 0.0f;
}

void SharedGrid :: setupGridCells()
{
    m_cells.resize(m_width);
    for(int i=0; i<m_width; i++)
    {
        m_cells[i].resize(m_height);
        for(int j=0; j<m_height; j++)
        {
            m_cells[i][j].m_position = glm::vec2(i, j);
            m_cells[i][j].grid_size = glm::vec2(m_width, m_height);
            m_cells[i][j].setFaces();
        }
    }


}

SharedCell& SharedGrid :: findCellByPos(glm::vec2 pos)
{
    return m_cells[pos.x][pos.y];
}

#pragma region GROUP
GroupGrid :: GroupGrid() : Grid(){}

GroupGrid :: GroupGrid(int width, int height) : Grid(width, height){}

void GroupGrid :: setupGridCells()
{
    m_cells.resize(m_width);
    for(int i=0; i<m_width; i++)
    {
        m_cells[i].resize(m_height);
        for(int j=0; j<m_height; j++)
        {
            m_cells[i][j].m_position = glm::vec2(i, j);
            m_cells[i][j].grid_size = glm::vec2(m_width, m_height);
            m_cells[i][j].setFaces();
        }
    }
}

GroupCell& GroupGrid :: findCellByPos(glm::vec2 pos)
{
    return m_cells[pos.x][pos.y];
}

void GroupGrid :: copyGridSize(SharedGrid &grid)
{
    m_width = grid.m_width;
    m_height = grid.m_height;
    setupGridCells();
}


#pragma endregion