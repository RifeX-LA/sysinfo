#pragma once

#include <windows.h>
#include <thread>

namespace flow::sysinfo::detail {

    std::size_t ft_to_size_t(const FILETIME& ft) {
        return ft.dwLowDateTime;
    }

    int logical_cores_number() {
        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        return static_cast<int>(sys_info.dwNumberOfProcessors);
    }

    int cpu_load() {
        FILETIME idle_time, prev_idle_time;
        FILETIME kernel_time, prev_kernel_time;
        FILETIME user_time, prev_user_time;

        GetSystemTimes(&prev_idle_time, &prev_kernel_time, &prev_user_time);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        GetSystemTimes(&idle_time, &kernel_time, &user_time);

        auto usr = ft_to_size_t(user_time) - ft_to_size_t(prev_user_time);
        auto ker = ft_to_size_t(kernel_time) - ft_to_size_t(prev_kernel_time);
        auto idl = ft_to_size_t(idle_time) - ft_to_size_t(prev_idle_time);

        auto sys = ker + usr;
        auto cpu_load = (sys - idl) * 100 / sys;

        return static_cast<int>(cpu_load);
    }

} // namespace flow::sysinfo::detail
