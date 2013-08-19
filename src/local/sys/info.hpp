#ifndef    page_local_sys_info_hpp
#   define page_local_sys_info_hpp

#	include <string>

#	include <boost/optional.hpp>

namespace page
{
	namespace sys
	{
		/**
		 * Write a summary of the available system information to the standard
		 * output stream.
		 */
		void PrintInfo();

		/**
		 * @return The name of the CPU.
		 */
		std::string GetCpu();

		/**
		 * @return The name of the operating system.
		 */
		std::string GetOs();

		/**
		 * @return The computer's hostname.
		 */
		std::string GetHostname();

		/**
		 * @return The name of the current user.
		 */
		std::string GetUsername();

		/**
		 * @return The path of the home directory.
		 */
		std::string GetHome();

		/**
		 * @param[in] key The key of the desired environment variable.
		 *
		 * @return The value of an environment variable.
		 */
		boost::optional<std::string> GetEnvVar(const std::string &key);
	}
}

#endif
