#pragma once

#include <format>
#include <iostream>
#include <flow/sysinfo/sysinfo_base.hpp>
#include <flow/sysinfo/detail/firewall.hpp>

namespace flow::sysinfo {

	struct firewall_t : sysinfo_base {
		bool is_enabled;

		explicit firewall_t(bool is_enabled) noexcept : is_enabled(is_enabled) {}

		void print() const override {
			std::cout << "Firewall:\n";
			std::cout << std::format("Is enabled: {}\n", is_enabled);
			std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("Firewall.Is enabled", is_enabled);
			return root;
		}
	};

	firewall_t firewall() {
		VARIANT_BOOL is_enabled;
		auto fw_profile = detail::firewall_profile();
		fw_profile->get_FirewallEnabled(&is_enabled);

		return firewall_t(is_enabled == VARIANT_TRUE);
	}

} // namespace flow::sysinfo
