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
