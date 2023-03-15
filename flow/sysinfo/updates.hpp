#pragma once

#include <format>
#include <iostream>
#include <flow/sysinfo/sysinfo_base.hpp>
#include <flow/sysinfo/detail/updates.hpp>

namespace flow::sysinfo {

	struct updates_t : sysinfo_base {
		AutoDownloadMode download_mode;

		explicit updates_t(AutoDownloadMode mode) noexcept: download_mode(mode) {}

		void print() const override {
			std::cout << "Updates:\n";
			std::cout << std::format("Auto-download mode: {}\n", detail::download_mode_to_str(download_mode));
            std::cout << std::format("{:-<20}\n", "");
		}

        [[nodiscard]] virtual boost::property_tree::ptree json() const override {
            boost::property_tree::ptree root;
            root.add("Updates.auto-download mode", detail::download_mode_to_str(download_mode));
            return root;
		}
	};

	updates_t updates() {
		IUpdate5* update;
		AutoDownloadMode mode;
		CoCreateInstance(CLSID_UpdateSession, nullptr, CLSCTX_INPROC_SERVER, IID_IUpdateSession, (LPVOID*)&update);
		update->get_AutoDownload(&mode);

		return updates_t(mode);
	}

} // namespace flow::sysinfo
