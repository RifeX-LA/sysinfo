#pragma once

#include <mutex>
#include <netfw.h>

namespace flow::sysinfo {

	class com {
		static std::once_flag m_init_flag;

		static void m_init() {
			CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
                                 nullptr, EOAC_NONE, nullptr);
		}

	public:
		com() {
			std::call_once(m_init_flag, m_init);
		}

		com(const com&) = delete;
		com& operator=(const com&) = delete;

		~com() {
			CoUninitialize();
		}
	};

	std::once_flag com::m_init_flag{};

} // namespace flow::sysinfo
