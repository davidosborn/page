#include <utility> // forward

#include "../../util/class/typeinfo.hpp" // GetIncompleteTypeInfo
#include "../../util/functional/cast.hpp" // reinterpret_cast_function

namespace page { namespace res
{
////////// Saver ///////////////////////////////////////////////////////////////

	template <typename T>
		Saver::Saver(const std::function<void (const T &, std::ostream &)> &f) :
			f(std::bind(f, std::bind(
				util::reinterpret_cast_function<const T &, const Referenceable &>(),
				std::placeholders::_1), std::placeholders::_2)) {}

	template <typename T>
		void Saver::operator ()(const T &t, std::ostream &os) const
	{
		(*this)(&t, os);
	}

////////// SaverRegistry ///////////////////////////////////////////////////////

	template <typename T, typename... RecordArgs>
		void SaverRegistry::Register(RecordArgs &&... recordArgs)
	{
		Register(
			util::GetIncompleteTypeInfo<T>(),
			Record(std::forward<RecordArgs>(recordArgs)...));
	}

	template <typename T>
		std::string SaverRegistry::Save(
			T           const& resource,
			std::string        path,
			std::string const& format,
			bool               bestMatch) const
	{
		return Save(
			util::GetIncompleteTypeInfo<T>(),
			&resource, path, format, bestMatch);
	}

	template <typename T>
		const typename SaverRegistry::Record &SaverRegistry::GetSaver(
			std::string const& format,
			std::string const& extension,
			bool               bestMatch) const
	{
		return GetSaver(
			util::GetIncompleteTypeInfo<T>(),
			format, extension, bestMatch);
	}
}}
