//
// Created by florenciaprado on 12/02/19.
//

#ifndef FULL_MENGE_PED_SIM_DENSITYFIELD_H
#define FULL_MENGE_PED_SIM_DENSITYFIELD_H


#include "Grid.h"
#include "MyModelAgent.h"

using namespace std;

namespace MyModel {

    class DensityField {
    public:

        float density_exponent;

        SharedGrid *m_grid;

        float m_min_density, m_max_density;

        /*!
        *	@brief		Constructor
        */
        DensityField();

        /*!
        *	@brief		Clears densities and velocities computed for each cell
        *               in the previous doStep
        */
        void clearDensitiesAndAVGVelocities();

        /*!
        *	@brief		Assigns densities and velocities to every cell.
        *               Every nearby agent provides a density to the closest four
        *               cells, and a equivalent part of his velocity
        */
        void assignDensitiesAndVelocities(vector<Agent> agents);

        /*!
        *	@brief		Calculated in a cell the density provided by nearby
        *   	        agents and added the velocities provided by each of these
        *  	            agents. These velocities are divided into the total computed
        *     	        density of the cell
        */
        void divideVelocities();

        /*!
        *	@brief		Updates the cells's velocity and density
        */
        void update(vector<Agent> agents);

        /*!
        *	@brief		Assigns min and max densities
        */

        void assignMinMax(float min, float max);

    };

}
#endif //FULL_MENGE_PED_SIM_DENSITYFIELD_H
