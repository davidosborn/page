// instance identification
// provides a unique identifier for each instance
// provides a mapping from identifier to instance
// similar in purpose to weak_ptr, but serializable and storage-independent
// NOTE: polymorphic with dynamic_cast to support multiple inheritance

#ifndef    page_local_util_Identifiable_hpp
#   define page_local_util_Identifiable_hpp

#	include <unordered_map>

namespace page
{
	namespace util
	{
		struct Identifiable
		{
			typedef unsigned long Id;

			// construct/copy/destroy/assign
			Identifiable();
			Identifiable(const Identifiable &);
			virtual ~Identifiable();
			Identifiable &operator =(const Identifiable &);

			// identification
			Id GetId() const;
			static Identifiable *FromId(Id);

			private:
			Id id;

			// identity generation
			static Id MakeId();

			// identity mapping
			typedef std::unordered_map<Id, Identifiable *> Ids;
			static Ids &GetIds();
		};

		// identification
		template <typename T> T *PointerFromId(Identifiable::Id);
		template <typename T> T &ReferenceFromId(Identifiable::Id);
	}
}

#	include "Identifiable.tpp"
#endif
