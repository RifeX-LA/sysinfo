#include <flow/sysinfo.hpp>
#include <flow/program_options.hpp>

template <typename Fn, typename... Args>
auto create_from(Fn&& fn, Args&&... args) {
    using invoke_result = std::invoke_result_t<Fn, Args...>;

    return std::make_unique<invoke_result>(std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...));
}

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	flow::sysinfo::com com;

	flow::sysinfo::info_table::inner_type inner;
    inner.emplace("cpu", create_from(flow::sysinfo::cpu));
    inner.emplace("ram", create_from(flow::sysinfo::ram));
    inner.emplace("disks", create_from(flow::sysinfo::disks));
    inner.emplace("updates", create_from(flow::sysinfo::updates));
    inner.emplace("fw", create_from(flow::sysinfo::firewall));
    inner.emplace("av", create_from(flow::sysinfo::antiviruses));

    flow::sysinfo::info_table info_table(std::move(inner));
	auto [desc, vm] = flow::sysinfo::options::parse(argc, argv);

	if (vm.count("help")) {
		std::cout << desc;
        return 0;
	}

    flow::sysinfo::options::apply_flags(vm, info_table);
    info_table.print();

    if (vm.count("json")) {
        flow::sysinfo::options::write_json(vm, info_table);
    }

    if (vm.count("size")) {
        std::locale::global(std::locale("en_US.UTF-8"));
        flow::sysinfo::options::print_sizes(vm);
    }

	return 0;
}
