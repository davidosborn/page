#include "ScopedRef.hpp"

namespace page
{
	namespace script
	{
		namespace python
		{
			// construct/destroy
			ScopedRef::ScopedRef(PyObject *obj) : obj(obj) {}
			ScopedRef::~ScopedRef()
			{
				Py_XDECREF(obj);
			}

			// modifiers
			void ScopedRef::Release()
			{
				obj = 0;
			}
			void ScopedRef::Reset(PyObject *obj)
			{
				Py_XDECREF(this->obj);
				this->obj = obj;
			}
		}
	}
}
