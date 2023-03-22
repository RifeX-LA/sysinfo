#pragma once

#include <boost/property_tree/ptree.hpp>

namespace flow::sysinfo {

	struct sysinfo_base {
		virtual void print() const = 0;
		[[nodiscard]] virtual boost::property_tree::ptree json() const = 0;
		virtual ~sysinfo_base() = default;
	};

} // namespace flow::sysinfo
