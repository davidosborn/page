#ifndef    page_local_util_class_special_member_functions_hpp
#   define page_local_util_class_special_member_functions_hpp

	/**
	 * Makes the destructor @c protected to prevent anyone from trying to delete
	 * a derived instance through a pointer to its base class, @c CLASS.  This
	 * macro must be called within the base class' definition.
	 *
	 * @param CLASS The affected class.
	 */
#	define DISABLE_DELETE_ON_POINTER_TO_BASE_CLASS(CLASS) \
		protected: \
		~CLASS() = default; \
		public:

	/**
	 * Defines the copy constructor and copy assignment operator of the class.
	 * This macro must be called within the class' definition.
	 *
	 * @param CLASS The affected class.
	 * @param VALUE The definition of the functions, either @c default or
	 *        @c delete.
	 */
#	define DEFINE_COPY(CLASS, VALUE) \
		CLASS(const CLASS &)             = VALUE; \
		CLASS &operator =(const CLASS &) = VALUE;

	/**
	 * Defines the move constructor and move assignment operator of the class.
	 * This macro must be called within the class' definition.
	 *
	 * @param CLASS The affected class.
	 * @param VALUE The definition of the functions, either @c default or
	 *        @c delete.
	 */
#	define DEFINE_MOVE(CLASS, VALUE) \
		CLASS(CLASS &&)             = VALUE; \
		CLASS &operator =(CLASS &&) = VALUE;

	/**
	 * Makes the class @e uncopyable (ISO/IEC N3337 8.4.3.3).  The class will
	 * not be copyable, but it will be movable using the default move
	 * constructor and move assignment operator.
	 *
	 * Derived classes will also not be copyable, and they will be moveable if
	 * the compiler is able to generate the default move constructor and move
	 * assignment operator (ISO/IEC N3337 12.8.9).
	 *
	 * The default constructor will not be implicitly generated.
	 */
#	define MAKE_UNCOPYABLE(CLASS) \
		public: \
		DEFINE_COPY(CLASS, delete) \
		DEFINE_MOVE(CLASS, default)

	/**
	 * Makes the class (and its derivatives) unmovable (and uncopyable).
	 *
	 * The default constructor will not be implicitly generated.
	 */
#	define MAKE_UNMOVABLE(CLASS) \
		public: \
		DEFINE_COPY(CLASS, delete) \
		DEFINE_MOVE(CLASS, delete)

namespace page { namespace util
{
	/**
	 * Makes the derived class polymorphic by giving it a virtual destructor,
	 * which makes it possible to delete a derived object through a pointer to
	 * its base class.
	 */
	template <typename Derived>
		struct Polymorphic
	{
		/**
		 * It is necessary to explicitly declare the default constructor because
		 * it will not be implicitly generated.
		 */
		Polymorphic() = default;

		/**
		 * @note Under GCC 4.7.1, the default destructor has a noexcept
		 *       exception specification.
		 *
		 * @note A loose exception specification is used here to permit derived
		 *       classes to also have a loose exception specification.
		 */
		virtual ~Polymorphic() = default;

		/**
		 * It is necessary to explicitly declare the copy constructor and
		 * assignment operator because they will not be implicitly generated.
		 */
		DEFINE_COPY(Polymorphic, default)

		/**
		 * It is necessary to explicitly declare the move constructor and
		 * assignment operator because they will not be implicitly generated.
		 */
		DEFINE_MOVE(Polymorphic, default)
	};

	/**
	 * Makes the derived class @e uncopyable (ISO/IEC N3337 8.4.3.3).  The
	 * derived class will not be copyable, but it will be moveable if the
	 * compiler is able to generate the default move constructor and
	 * move assignment operator (ISO/IEC N3337 12.8.9).
	 */
	template <typename Derived>
		struct Uncopyable
	{
		/**
		 * It is necessary to explicitly declare the default constructor
		 * because it will not be implicitly generated.
		 */
		Uncopyable() = default;

		DISABLE_DELETE_ON_POINTER_TO_BASE_CLASS(Uncopyable)

		MAKE_UNCOPYABLE(Uncopyable)
	};

	/**
	 * Makes the derived class unmovable (and uncopyable).
	 */
	template <typename Derived>
		struct Unmovable : Uncopyable<Derived>
	{
		/**
		 * It is necessary to explicitly declare the default constructor
		 * because it will not be implicitly generated.
		 */
		Unmovable() = default;

		DISABLE_DELETE_ON_POINTER_TO_BASE_CLASS(Unmovable)

		MAKE_UNMOVABLE(Unmovable)
	};
}}

#endif
