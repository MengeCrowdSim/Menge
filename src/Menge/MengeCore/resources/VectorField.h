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

/*!
 @file  VectorField.h
 @brief  The definition of a 2D vector field.
 */

#ifndef __VECTOR_FIELD_H__
#define __VECTOR_FIELD_H__

#include "MengeCore/mengeCommon.h"
#include "MengeCore/resources/Resource.h"

#include <map>
#include <string>

namespace Menge {

/*!
 @brief    A simple 2D vector field.

 The field is defined by the location of its bottom, left-hand corner, the size of the space the
 grid should cover and the size of each, square cell.
 */
class MENGE_API VectorField : public Resource {
 public:
  /*!
   @brief    Default constructor.

   @param    fileName    The name of the file which contains the vector field definition.
   */
  VectorField(const std::string& fileName);

 protected:
  /*!
   @brief    Destructor.
   */
  virtual ~VectorField();

 public:
  /*!
   @brief    Returns a unique resource label to be used to identify different resource *types* which
            use the same underlying file data.
   */
  virtual const std::string& getLabel() const { return LABEL; }

  /*!
   @brief    Reports the cell the a point is in.
   
   @param    pos    The point to test.
   @param    r      A reference to the row index -- this is to be set by the function.
   @param    c      A reference to the column index -- this is to be set by the function.
   */
  // TODO: Determine what happens if pos is off the grid
  void getCell(const Math::Vector2& pos, int& r, int& c);

  /*!
   @brief    Returns the value of the field for the given CELL address

   The row and column values are only validated in debug mode, using an assertion.

   @param    row    The index of the row
   @param    col    The index of the column.
   @returns  The vector value of the cell at (row, col)
   */
  Math::Vector2 getFieldValue(int row, int col) const;

  /*!
   @brief    Returns the value of the field for the given position.

   This uses a "nearest" value look-up. The cell center which is closest to the query point is used.
   That means, points that lie outside the field will get the vector value of the closest cell in
   the grid.

   @param    pos    The position to read the field's vector value.
   @returns  The vector value of the cell center closest to pos.
   */
  Math::Vector2 getFieldValue(const Math::Vector2& pos);

  /*!
   @brief    Returns the value of the field for the given position.

   This uses a bi-linear interpolation value. The four surrounding cells' values are combined
   linearly based on relative distances. If the point lies outside the field, then only two, or
   possibly only one cell is used.

   @param    pos    The position to read the field's vector value.
   @returns  The vector value of the cell center closest to pos.
   */
  Math::Vector2 getFieldValueInterp(const Math::Vector2& pos);

  /*!
   @brief    Parses a vector field definition and returns a pointer to it.

   This function works in conjunction with the ResourceManager. That is why it returns a pointer,
   not to a VectorField, but to a Resource. The ResourceManager uses it to load and instantiate
   VectorField instances.

   @param    fileName    The path to the file containing the VectorField definition.
   @returns  A pointer to the new VectorField (if the file is valid), NULL if invalid.
   */
  static Resource* load(const std::string& fileName);

  /*!
   @brief    Reports the minimum extent of the field.

   @returns  The minimum extent of the field.
   */
  Math::Vector2 getMinimumPoint() const { return _minPoint; }

  /*!
   @brief    Reports the maximum extent of the field.

   @returns  The maximum extent of the field.
   */
  Math::Vector2 getMaximumPoint() const;

  /*!
   @brief    Reports the size of the field (along both axes).

   @returns  The size of the field.
   */
  Math::Vector2 getSize() const;

  /*!
   @brief    Reports the number of rows in the field.

   @returns  The number of rows.
   */
  inline int getRowCount() const { return _resolution[0]; }

  /*!
   @brief    Reports the number of columns in the field.

   @returns  The number of columns.
   */
  inline int getColCount() const { return _resolution[1]; }

  /*!
   @brief    Reports the number of rows in the field.

   @returns  The number of rows.
   */
  inline float getCellSize() const { return _cellSize; }

  /*!
   @brief    Overloaded streaming output operator to print the field to an output stream.

   @param    out    The output stream.
   @param    vf    The vector field to convert to a string.
   @returns  The output stream.
   */
  friend Logger& operator<<(Logger& out, const VectorField& vf);

  /*!
   @brief    The unique label for this data type to be used with
   resource management.
   */
  static const std::string LABEL;

 protected:
  /*!
   @brief    The minimum extent of the vector field.
   */
  Math::Vector2 _minPoint;

  /*!
   @brief    The resolution (in cells) of the field's grid (rowCount, colCount)
   */
  int _resolution[2];

  /*!
   @brief    Size of the grid cell.
   */
  float _cellSize;

  /*!
   @brief    The 2D array of vector data for each cell.
   */
  Math::Vector2** _data;

  /*!
   @brief    Computes the appropriate resolution of the grid.

   It uses the pre-determined cell size to determine the resolution that tightly spans the size
   given (although, if the size is not an integer multiple of the cell size, the resultant field
   will be *slightly* larger (i.e. the extra extent will be less than the cell size).

   @param    width      The desired size of the field along the x-axis.
   @param    height    The desired size of the field along the y-axis.
   */
  void setDimensions(float width, float height);

  /*!
   @brief    Given the stored resolution, intializes the data array.
   */
  void initDataArray();

  /*!
   @brief    frees the data array.
   */
  void freeDataArray();
};

/*!
 @brief    The definition of the managed pointer for VectorField data
 */
typedef ResourcePtr<VectorField> VectorFieldPtr;

/*!
 @brief    Loads the vector field of the given name

 @param  fileName  The name of the file containing the vector field definition.
 @returns  The VectorFieldPtr containing the data.
 @throws    A ResourceException if the data is unable to be instantiated.
 */
VectorFieldPtr loadVectorField(const std::string& fileName) throw(ResourceException);
}  // namespace Menge

#endif  // __VECTOR_FIELD_H__
