#pragma once

#include <filesystem>
#include <string_view>
#include <ranges>
#include <format>
#include <iostream>
#include <fileapi.h>
#include <flow/sysinfo/sysinfo_base.hpp>

namespace flow::sysinfo {

	struct disk_t : sysinfo_base {
		std::size_t total;
		std::size_t available;
		std::string name;

		disk_t(std::size_t t, std::size_t a, std::string n) : total(t), available(a), name(std::move(n)) {}

		void print() const override {
			std::cout << std::format("Name: {}\n", name);
			std::cout << std::format("Total: {:.3} Gb\n", total / std::pow(1024, 3));
			std::cout << std::format("Available: {:.3} Gb", available / std::pow(1024, 3));
		}

		[[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("Disk.name", name);
            root.add("Disk.total", total);
            root.add("Disk.available", available);
            return root;
		}
	};

	struct disks_t : sysinfo_base {
		std::vector<disk_t> disks;

		explicit disks_t(const std::vector<disk_t>& d) : disks(d) {}
		explicit disks_t(std::vector<disk_t>&& d) noexcept: disks(std::move(d)) {}

		void print() const override {
			std::cout << std::format("Disks ({}):", disks.size());
            std::ranges::for_each(disks, [](auto&& disk){std::cout << "\n\n"; disk.print();});
			std::cout << std::format("\n{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            boost::property_tree::ptree disks_nodes;
            std::ranges::for_each(disks, [&](auto&& disk){disks_nodes.push_back(disk.json().front());});
            root.add_child("Disks", disks_nodes);

            return root;
		}
	};

	disks_t disks() {
		static constexpr char sep = '\0';

		char raw_disks_names[256];
		auto len = GetLogicalDriveStringsA(std::size(raw_disks_names), raw_disks_names);

		std::ranges::subrange names_range(raw_disks_names, raw_disks_names + len - 1);
		auto disks_names = std::views::split(names_range, sep) |
						   std::views::transform([](auto&& view){return std::string_view(view.begin(), view.end());});

		std::vector<disk_t> info;
		for (auto&& name : disks_names) {
			auto space_info = std::filesystem::space(name);
			info.emplace_back(space_info.capacity, space_info.available, name.data());
		}

		return disks_t(std::move(info));
	}

} // namespace flow::sysinfo
