/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_phys_attrib_Deformation_hpp
#   define page_local_phys_attrib_Deformation_hpp

#	include <unordered_map>

#	include "PositionNormal.hpp"
#	include "TexCoord.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The vertex deformation part of a node.
	 */
	class Deformation
	{
		public:
		/**
		 * An individual vertex, as part of a vertex deformation.
		 */
		class Vertex :
			public PositionNormal,
			public TexCoord
		{
			/*--------------------+
			| frame serialization |
			+--------------------*/

			protected:
			Frame GetFrame() const;
			void SetFrame(const Frame &);
		};

		/*----------+
		| observers |
		+----------*/

		/**
		 * Returns @c true if any vertices have been deformed.
		 */
		bool IsDeformed() const;

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Updates the state of all deformed vertices.
		 */
		void UpdateDeformation();

		/**
		 * Clears all vertices of their deformation state.
		 */
		void ResetDeformation();

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::unordered_map<unsigned, Vertex> vertices;
	};
}}}

#endif
