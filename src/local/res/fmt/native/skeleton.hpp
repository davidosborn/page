/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_res_fmt_native_skeleton_hpp
#   define page_res_fmt_native_skeleton_hpp

#	include <cstdint> // {,u}int32_t

namespace page
{
	namespace res
	{
		namespace fmt
		{
			namespace native
			{
				namespace skeleton
				{
					const char sig[] = {'P', 'A', 'G', 'E', 's', 'k', 'e', 'l'};

#	pragma pack(push, 1)
					struct Header
					{
						std::uint32_t bones;
					};
					struct BoneName
					{
						std::uint32_t size;
					};
					struct Bone
					{
						float position[3];
						float orientation[3]; // quaternion.xyz, normalized w
						std::int32_t parent;
					};
#	pragma pack(pop)

					const char headerFormat[] = "d";
					const char boneNameFormat[] = "d";
					const char boneFormat[] = "ddddddd";
				}
			}

			using namespace native::skeleton;
		}
	}
}

#endif
