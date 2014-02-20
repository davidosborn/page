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

#ifndef    page_local_util_Identifiable_hpp
#   define page_local_util_Identifiable_hpp

#	include <cstddef> // size_t

#	include "class/special_member_functions.hpp" // Polymorphic

namespace page { namespace util
{
	/**
	 * A mixin which maps each instance to a unique identifier.  The identifier
	 * is unique across all derived classes, and is serializable and storage
	 * independent.
	 *
	 * @note This class is polymorphic to allow dynamic_cast.
	 */
	class Identifiable : public Polymorphic<Identifiable>
	{
		public:
		typedef std::size_t Id;

		/*-------------+
		| constructors |
		+-------------*/

		Identifiable();
		Identifiable(const Identifiable &);
		~Identifiable() override;
		Identifiable &operator =(const Identifiable &);

		/*---------------+
		| identification |
		+---------------*/

		/**
		 * Returns the ID associated with this instance.
		 */
		Id GetId() const;

		/**
		 * Returns the instance associated with the specified ID.
		 */
		static Identifiable *GetInstance(Id);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The ID associated with this instance.
		 */
		Id id;
	};

	/*-------------------------+
	| type-safe identification |
	+-------------------------*/

	/**
	 * Returns a pointer to the instance associated with the specified ID,
	 * downcasted to the specified type.  If the instance is not based on the
	 * specified type, returns nullptr.
	 */
	template <typename T>
		T *GetPointerById(Identifiable::Id);

	/**
	 * Returns a reference to the instance associated with the specified ID,
	 * downcasted to the specified type.  If the instance is not based on the
	 * specified type, throws an exception.
	 */
	template <typename T>
		T &GetReferenceById(Identifiable::Id);
}}

#	include "Identifiable.tpp"
#endif
