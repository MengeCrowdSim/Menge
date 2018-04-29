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
 @file    GLLight.h
 @brief   A simple light for defining OpenGL lighting properties.
 */

#ifndef __GL_LIGHT_H__
#define __GL_LIGHT_H__

#include <vector>
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {
/*!
 @brief   The OpenGL light class.
 
 Defines a light for the basic OpenGL pipeline.
 */
class MENGEVIS_API GLLight {
 public:
  /*!
   @brief   Defines the space of the camera: world or camera.

   If world, the camera is fixed to the world. If camera, it is fixed to the camera and moves with
   the camera.
   */
  enum LightSpace { WORLD, CAMERA };

  /*!
   @brief   Defines the type of the light: point or directional.

   If the light is point light, it acts like a single point source originating from the position. If
   the light is a directional light, it is a light source, infinitely far away oriented in the
   direction of the vector defined by the light position.
   */
  enum LightType { POINT, DIRECTIONAL };

  /*!
   @brief   Default constructor
   */
  GLLight();

  /*!
   @brief   Sets the default light parameters
   */
  void setDefaults();

  /*!
   @brief   Sets the light color.

   @param   r   The red component of the light's diffuse color. Must lie in the range [0, 1].
   @param   g   The green component of the light's diffuse color. Must lie in the range [0, 1].
   @param   b   The blue component of the light's diffuse color. Must lie in the range [0, 1].
   @param   a   The alpha component of the light's diffuse color. Must lie in the range [0, 1].
   */
  void setColor(float r, float g, float b, float a = 1.0);

  /*!
   @brief   Sets the light color.

   @param   x    The x-value of the light's position.
   @param   y    The y-value of the light's position.
   @param   z    The z-value of the light's position.
   @param   w    The w-value of the light's position. It determines the light type (point or
                 directional). If the value is 0, the light is set to directional, if > 0 it is set
                 to point, if < 0, it remains unchanged.
   */
  void setPosition(float x, float y, float z, float w = -1.f);

  /*!
   @brief   Sets the light type

   @param   lightType   The enumeration specifying either POINT or DIRECTIONAL light.
   */
  void setLightType(LightType lightType);

  /*!
   @brief   Sets the space of the camera

   @param   space   The space for the camera.
   */
  void setSpace(LightSpace space) { _space = space; }

  /*!
   @brief   Initializes the corresponding OpenGL light.

   @param   i       The GL identifier for this light, such that this light is GL_LIGHTi.
   @param   space   The space being initialized.  If the light is defined in a different space, then
                    it shouldn't execute itself.
   */
  void initGL(int i, LightSpace space = WORLD) const;

 protected:
  /*!
   @brief   The diffuse color of the light. It includes the red, green, blue and alpha values.
   */
  float _diff[4];

  /*!
   @brief   The position of the light. It includes the x, y, z and type values. If type is 0, the
            light is directional, otherwise it is a point light.
   */
  float _pos[4];

  /*!
   @brief   Space of the light.
   */
  LightSpace _space;
};

/*!
 @brief   An STL vector of GLLight objects.
 */
typedef std::vector<GLLight> GLLightVector;

/*!
 @brief   An iterator for the GLLightVector.
 */
typedef GLLightVector::iterator GLLightVectorItr;

/*!
 @brief   A const iterator for the GLLightVector.
 */
typedef GLLightVector::const_iterator GLLightVectorCItr;
}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __GL_LIGHT_H__
