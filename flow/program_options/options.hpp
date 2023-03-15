#pragma once

#include <boost/program_options.hpp>
#include <flow/program_options/info_table.hpp>

namespace flow::sysinfo::options {

	std::pair<boost::program_options::options_description, boost::program_options::variables_map>
	parse(int argc, char* argv[]) {
		namespace po = boost::program_options;

		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Show help")
			("cpu", "Show CPU info")
			("ram", "Show RAM info")
			("disks", "Show disks info")
			("fw", "Show firewall info")
			("updates", "Show updates info")
			("av", "Show antiviruses info")
			("no-cpu", "Don't show CPU info")
			("no-ram", "Don't show RAM info")
			("no-disks", "Don't show disks info")
			("no-fw", "Don't show firewall info")
			("no-updates", "Don't show updates info")
			("no-av", "Don't show antiviruses info")
			("json", po::value<std::string>(), "Parse to json")
            ("size,s", po::value<std::vector<std::string>>(), "Get size of folder or file (in bytes)");

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
		po::notify(vm);

		return {std::move(desc), std::move(vm)};
	}

    std::size_t dir_size(const std::filesystem::path& path) {
        std::size_t size = 0;
        for (auto&& entry : std::filesystem::recursive_directory_iterator(path)) {
            size += std::filesystem::file_size(entry);
        }

        return size;
    }

    void apply_flags(const boost::program_options::variables_map& vm, info_table& info_table) {
        bool unset_required = true;
        std::ranges::for_each(vm, [&](auto&& item) {
            if (info_table.apply(item.first, unset_required)) {
                unset_required = false;
            }
        });
    }

    void write_json(const boost::program_options::variables_map& vm, const info_table& info_table) {
        auto path = vm["json"].as<std::string>();
        if (!path.ends_with(".json")) {
            path += ".json";
        }
        info_table.write_json(path);
    }

    void print_sizes(const boost::program_options::variables_map& vm) {
        namespace fs = std::filesystem;

        auto paths = vm["size"].as<std::vector<std::string>>();
        std::cout << "Sizes:\n";
        for (auto&& path : paths) {
            if (fs::exists(path)) {
                std::size_t size = fs::is_directory(path) ? dir_size(path) : fs::file_size(path);
                std::cout << std::format("{}: {:L} bytes\n", path, size);
            }
            else {
                std::cout << std::format("{}: not exists\n", path);
            }
        }
    }

} // namespace flow::sysinfo::options
