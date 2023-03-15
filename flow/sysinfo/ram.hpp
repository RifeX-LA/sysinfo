#pragma once

#include <format>
#include <iostream>
#include <windows.h>
#include <flow/sysinfo/sysinfo_base.hpp>

namespace flow::sysinfo {

	struct ram_t : sysinfo_base {
		std::size_t volume;
		std::size_t load;

		ram_t(std::size_t vol, std::size_t l) noexcept: volume(vol), load(l) {}

		void print() const override {
			std::cout << "RAM:\n";
			std::cout << std::format("Volume: {} Mb\nLoad: {}%\n", volume / 1024 / 1024, load);
			std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("RAM.volume", volume);
            root.add("RAM.load", load);
			return root;
		}
	};

	ram_t ram() {
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);

		return ram_t(statex.ullTotalPhys, statex.dwMemoryLoad);
	}

} // namespace flow::sysinfo
