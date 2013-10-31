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
