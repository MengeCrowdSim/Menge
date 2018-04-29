/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

#ifndef __HEIGHTFIELD_H__
#define __HEIGHTFIELD_H__

/*!
 @file       HeightField.h
 @brief      Definition of a height field based on a uniform discretization of space.
 */

#include "MengeCore/Math/Vector3.h"
#include "MengeCore/resources/Resource.h"
#include "TerrainConfig.h"

#include <string>

namespace Terrain {
/*!
 @brief		A heightfield.  A uniform discretization of space which supports queries on height and
          normal of field.
 */
class EXPORT_API HeightField : public Menge::Resource {
 public:
  /*!
   @brief		Constructor.

   @param		fileName		The height field specification file used to define the height field.
   */
  HeightField(const std::string& fileName);

 protected:
  /*!
   @brief		Destructor
   */
  ~HeightField();

 public:
  /*!
   @brief		Initialize the height field.  The heightfield is positioned with the "minimum" corner at
            the origin and extending into the first quadrant of the x-z plane based on cell size and
            image resolution.

   @param		imgName			  The image file used to define the height field.
   @param		cellSize		  The size of each cell in the image in world coordinates.
   @param		vertScale		  The values of the image (in the range [0, 255] are normalized to the range
                          [0, 1] and then multiplied by this vertical scale.
   @param		xpos			    The x-coordinate of the terrain's origin
   @param		zpos			    The z-coordinate of the terrain's origin
   @param		smoothParam		The smoothing parameter for the elevation values. It is interpreted as the
                          standard deviation of a 2D symmetric gaussian kernel. If zero, no
                          smoothing will be performed.
   @returns	true if initalization was successful
   */
  bool initialize(const std::string& imgName, float cellSize, float vertScale, float xpos,
                  float zpos, float smoothParam = 0.f);

  /*!
   @brief		Returns a unique resource label to be used to identify different resource *types* which
            use the same underlying file data.
   */
  virtual const std::string& getLabel() const { return LABEL; }

  /*!
   @brief		Parses a graph definition and returns a pointer to it.

   This function works in conjunction with the ResourceManager. That is why it returns a pointer,
   not to a Graph, but to a Resource. The ResourceManager uses it to load and instantiate Graph
   instances.

   @param		fileName		The path to the file containing the VectorField definition.
   @returns	A pointer to the new Graph (if the file is valid), NULL if invalid.
   */
  static Resource* load(const std::string& fileName);

  /*!
   @brief		Given the height field information, computes normals for the data.
   */
  void computeNormals();

  /*!
   @brief		Returns the height at the given world position. If the world position lies outside of
            the domain of the height field, the height at the nearest cell center is returned.

   @param		x		The position along the x-axis.
   @param		y		The position along the y-axis.
   @returns	The height at the position (x, y).
   */
  float getHeightAt(float x, float y) const;

  /*!
   @brief		Returns the height field normal at the given world position.
   
   If the world position lies outside of the domain of the height field, the normal of the nearest
   cell center is returned.

   @param		x		The position along the x-axis.
   @param		y		The position along the y-axis.
   @returns	The normal at the position (x, y).
   */
  Menge::Math::Vector3 getNormalAt(float x, float y) const;

  /*!
   @brief		Returns the height at the given cell center.

   The behavior is undefined if the indices fall outside the array of cell values.

   @param		x		The index along the x-axis.
   @param		y		The index along the y-axis.
   @returns	The height at the cell center with indices[x, y].
   */
  float getHeightAtCell(int x, int y) const;

  /*!
   @brief		Returns the normal at the given cell center.

   The behavior is undefined if the indices fall outside the array of cell values.

   @param		x		The index along the x-axis.
   @param		y		The index along the y-axis.
   @returns	The normal at the cell center with indices[x, y].
   */
  Menge::Math::Vector3 getNormalAtCell(int x, int y) const;

  /*!
   @brief		Return the number of cells in the width direction of the field.

   @returns	The number of cells in the width (x) direction.
   */
  int getW() const { return _W; }

  /*!
   @brief		Return the number of cells in the height direction of the field.

   @returns	The number of cells in the height (y) direction.
   */
  int getH() const { return _H; }

  /*!
   @brief		Returns the cellSize of the height field.

   @returns	The size of the side of the square cell.
   */
  float getCellSize() const { return _cellSize; }

  /*!
   @brief		Returns the x-position of the mininum corner of the grid.

   @returns	The minimum corner of the grid.
   */
  float getCornerX() const { return _xpos; }

  /*!
   @brief		Returns the y-position of the mininum corner of the grid.

   @returns	The minimum corner of the grid.
   */
  float getCornerY() const { return _ypos; }

  /*!
   @brief		The unique label for this data type to be used with resource management.
   */
  static const std::string LABEL;

 protected:
  /*!
   @brief		Smooth the elevation using a symmetric 2D gaussian kernel.

   @param		smooth		The smoothing parameter (standard deviation) of the kernel.
   */
  void smoothElevation(float smooth);

  /*!
   @brief		The size of a cell in the heightfield (in world coordinates)
   */
  float _cellSize;

  /*!
   @brief		The number of cells in the width (x) direction.
   */
  int _W;

  /*!
   @brief		The number of cells in the height (z) direction.
   */
  int _H;

  /*!
   @brief		The data for the height field.
   */
  float** _heightMap;

  /*!
   @brief		The data for the normals of the height field.
   */
  Menge::Math::Vector3** _normalMap;

  /*!
   @brief		The x-position of the minimum corner of the heightfield.
   */
  float _xpos;

  /*!
   @brief		The y-position of the minimum corner of the heightfield.
   */
  float _ypos;
};

/*!
 @brief		The definition of the managed pointer for HeightField data
 */
typedef Menge::ResourcePtr<HeightField> HeightFieldPtr;

/*!
 @brief		Loads the height field of the given name

 @param		fileName		The name of the file containing the height field.
 @returns	The HeightFieldPtr containing the data.
 @throws		A ResourceException if the data is unable to be instantiated.
 */
HeightFieldPtr loadHeightField(const std::string& fileName) throw(Menge::ResourceException);
}  // namespace Terrain
#endif
