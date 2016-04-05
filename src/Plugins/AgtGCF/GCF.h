/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

/*!
 *	@file		GCF.h
 *	@brief		The definition of the Generalized Centrifugal Force pedestrian plug-in.
 *
 *	This is inspired by the paper by Chraibi et al (Generalized Centrifugal Force model.
 *	However, the implementation differs in several critical ways:
 *
 *		1) The "facing" direction is not simply the moving direction.  In fact, this
 *		   model has a unique computation for orientation based on its movement
 *		   speed.
 *		2) Rather than having a facing direction that simply cuts off at 90 degrees
 *		   to the side of the facing direction, forces are generated for agents in
 *		   all directions, however, the magnitudes of those forces is modulated by
 *		   how near the facing direction the neighbor is -- the closer to directly
 *		   in front, the larger the force.
 *		3) The repulsion force is maxed out at 3X the user-specified maximum agent
 *		   force.  To insure a well-behaved response function, the interpolation
 *		   width is constrained such that the response force only grows monotonically.
 *		4) The magnitude of the velocity scale (v_ij) is not the dot product of the
 *		   relative velocity and orientation, but of the velocity and the *force*
 *		   direction.  Essentially, this factor goes to zero if the agents are already
 *		   diverging.  Only if the agents are *converging* does this factor increase.
 */

#ifndef __GCF_H__
#define __GCF_H__

#include "GCFAgent.h"
#include "GCFSimulator.h"

#endif	//__GCF_H__