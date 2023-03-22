#pragma once

#include <format>
#include <iostream>
#include <flow/sysinfo/sysinfo_base.hpp>
#include <flow/sysinfo/detail/firewall.hpp>

namespace flow::sysinfo {

	struct firewall_t : sysinfo_base {
		bool enabled;

		explicit firewall_t(bool is_enabled) noexcept : enabled(is_enabled) {}

		void print() const override {
			std::cout << "Firewall:\n";
			std::cout << std::format("Enabled: {}\n", enabled);
			std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("Firewall.Enabled", enabled);

			return root;
		}
	};

	firewall_t firewall() {
		VARIANT_BOOL enabled;
		auto fw_profile = detail::firewall_profile();
		fw_profile->get_FirewallEnabled(&enabled);

		return firewall_t(enabled == VARIANT_TRUE);
	}

} // namespace flow::sysinfo
