#ifndef    page_local_err_report_hpp
#   define page_local_err_report_hpp

#	include <exception>

namespace page { namespace err
{
	/**
	 * Print an error message from an exception.
	 */
	void ReportError(const std::exception &);

	/**
	 * Print a warning message from an exception.
	 */
	void ReportWarning(const std::exception &);
}}

#endif
