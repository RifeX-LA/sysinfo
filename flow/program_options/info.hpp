#pragma once

#include <memory>
#include <flow/sysinfo/sysinfo_base.hpp>

namespace flow::sysinfo {

	class info {
		bool m_print_needed = true;
		std::unique_ptr<sysinfo_base> m_sysinfo;

	public:
		info() = default;

		template <typename T>
		explicit info(std::unique_ptr<T>&& sysinfo) noexcept: m_sysinfo(std::move(sysinfo)) {}

		void set() noexcept {
			m_print_needed = true;
		}

		void unset() noexcept {
			m_print_needed = false;
		}

		[[nodiscard]] bool flag() const noexcept {
			return m_print_needed;
		}

		void print() const {
			if (m_print_needed) {
				m_sysinfo->print();
			}
		}

        [[nodiscard]] virtual boost::property_tree::ptree json() const {
			return m_sysinfo->json();
		}
	};
	
} // namespace flow::sysinfo 
