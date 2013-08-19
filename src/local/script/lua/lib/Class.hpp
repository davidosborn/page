#ifndef    page_local_script_lua_lib_Class_hpp
#   define page_local_script_lua_lib_Class_hpp

#	include <functional> // function
#	include <memory> // shared_ptr

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Class
	{
		// construct
		explicit Class(lua_State *);

		// instance mapping
		/*
		RegisterInstance   [-1, 0, e]

			Pops a table from the top of the stack and registers an
			association between it and the instance pointer.  The
			pointer and deleter are wrapped in userdata, with the
			deleter bound to the __gc metamethod.

		DeleteInstance     [-1, 0, e]

			Pops a table from the top of the stack and deletes any
			registered association between it and any instance.
			Also calls the instance's deleter and removes the
			userdata's __gc metamethod.

		GetInstance        [-1, 0, e]

			Pops a table from the top of the stack and returns the
			registered instance associated with it, or a null
			pointer.
		*/
		static void RegisterInstance(lua_State *, const std::shared_ptr<void> &);
		static void DeleteInstance(lua_State *);
		static void *GetInstance(lua_State *);

		// property registration
		/*
		Property

			Similar to luaL_Reg, but for properties to be registered
			by RegisterProperties.  getter or setter can be null to
			make the property ungettable or unsettable.

		RegisterProperties   [0, 0, m]

			Similar to luaL_register, but without package.loaded.
			Registers properties to the __getter and __setter fields
			of the given table.  The table name cannot be null.
		*/
		struct Property
		{
			const char *name;
			lua_CFunction getter, setter;
		};
		static void RegisterProperties(lua_State *, const char *, const Property *);

		private:
		// functions
		static int EnableDelete(lua_State *);
	};
}}}}

#endif
