#ifndef    page_local_phys_attrib_Deformation_hpp
#   define page_local_phys_attrib_Deformation_hpp

#	include <unordered_map>

#	include "PositionNormal.hpp"
#	include "TexCoord.hpp"

namespace page { namespace phys { namespace attrib
{
	class Deformation
	{
		struct Vertex :
			PositionNormal,
			TexCoord
		{
			protected:
			/*--------------------+
		| frame serialization |
		+--------------------*/

			Frame GetFrame() const;
			void SetFrame(const Frame &);
		};

		// status
		bool IsDeformed() const;

		// modifiers
		void UpdateDeformation();
		void ResetDeformation();

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		private:
		typedef std::unordered_map<unsigned, Vertex> Vertices;
		Vertices vertices;
	};
}}}

#endif
