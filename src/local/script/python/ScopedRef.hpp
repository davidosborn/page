#ifndef    page_local_script_python_ScopedRef_hpp
#   define page_local_script_python_ScopedRef_hpp

#	include <Python.h> // PyObject

#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace script
	{
		namespace python
		{
			struct ScopedRef : util::Uncopyable<ScopedRef>
			{
				// construct/destroy
				ScopedRef(PyObject * = 0);
				~ScopedRef();

				// modifiers
				void Release();
				void Reset(PyObject * = 0);

				private:
				PyObject *obj;
			};
		}
	}
}

#endif
