#pragma once

#include <format>
#include <iostream>
#include <flow/sysinfo/sysinfo_base.hpp>
#include <flow/sysinfo/detail/cpu.hpp>

namespace flow::sysinfo {

	struct cpu_t : sysinfo_base {
		int cores;
		int load;

		cpu_t(int c, int l) noexcept: cores(c), load(l) {}

		void print() const override {
			std::cout << "CPU:\n";
			std::cout << std::format("Logical cores: {}\nLoad: {}%\n", cores, load);
			std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
			boost::property_tree::ptree root;
            root.add("CPU.Logical cores", cores);
            root.add("CPU.Load", load);

            return root;
		}
	};

	cpu_t cpu() {
		return cpu_t(detail::logical_cores_number(), detail::cpu_load());
	}

} // namespace flow::sysinfo
