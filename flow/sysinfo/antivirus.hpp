#pragma once

#include <vector>
#include <format>
#include <iostream>
#include <flow/sysinfo/sysinfo_base.hpp>
#include <flow/sysinfo/detail/antivirus.hpp>

namespace flow::sysinfo {

	struct antivirus_t : sysinfo_base {
		int state;
		std::string name;

		antivirus_t(int state, std::string n): state(state), name(std::move(n)) {}

		void print() const override {
			std::cout << std::format("Name: {}\n", name);
			std::cout << std::format("State: {} ({:b})\n", state, state);
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("Antivirus.name", name);
            root.add("Antivirus.state", state);

            return root;
		}
	};

	struct antiviruses_t : sysinfo_base {
		std::vector<antivirus_t> antiviruses;

		explicit antiviruses_t(const std::vector<antivirus_t>& a) : antiviruses(a) {}
		explicit antiviruses_t(std::vector<antivirus_t>&& a) noexcept : antiviruses(std::move(a)) {}

		void print() const override {
			std::cout << std::format("Antiviruses ({}):", antiviruses.size());
            std::ranges::for_each(antiviruses, [](auto&& antivirus) {std::cout << "\n\n"; antivirus.print();});
			std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            boost::property_tree::ptree av_nodes;
            std::ranges::for_each(antiviruses, [&](auto&& av){av_nodes.push_back(av.json().front());});
            root.add_child("Antiviruses", av_nodes);

            return root;
		}
	};

    antiviruses_t antiviruses() {
        CComPtr<IWbemLocator> locator;
        locator.CoCreateInstance(CLSID_WbemLocator);

        CComPtr<IWbemServices> services;
        locator->ConnectServer(CComBSTR(L"root\\SecurityCenter2"), nullptr, nullptr,
                               nullptr, NULL, nullptr, nullptr, &services);

        CComPtr<IEnumWbemClassObject> enumerator;
        CComBSTR query = L"Select * From AntivirusProduct";
        services->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY, nullptr, &enumerator);

        std::vector<antivirus_t> info;

        while (enumerator) {
            ULONG returned = 0;
            CComPtr<IWbemClassObject> object = nullptr;

            enumerator->Next(WBEM_INFINITE, 1, &object, &returned);
            if (returned == 0) {
                break;
            }

            auto [state, name] = detail::antivirus_info(object);
            info.emplace_back(state, std::move(name));
        }

        return antiviruses_t(std::move(info));
    }

} // namespace flow::sysinfo
