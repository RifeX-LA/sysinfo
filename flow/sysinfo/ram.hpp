#pragma once

#include <format>
#include <iostream>
#include <windows.h>
#include <flow/sysinfo/sysinfo_base.hpp>

namespace flow::sysinfo {

	struct ram_t : sysinfo_base {
        unsigned load;
		std::uintmax_t volume;

		ram_t(unsigned l, std::uintmax_t vol) noexcept: load(l), volume(vol) {}

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
		MEMORYSTATUSEX mem_status;
        mem_status.dwLength = sizeof(mem_status);
		GlobalMemoryStatusEx(&mem_status);

		return ram_t(mem_status.dwMemoryLoad, mem_status.ullTotalPhys);
	}

} // namespace flow::sysinfo
