#pragma once

#include <netfw.h>
#include <flow/sysinfo/detail/releaser.hpp>

namespace flow::sysinfo::detail {

    std::unique_ptr<INetFwProfile, releaser> firewall_profile() {
        auto mgr = make_with_releaser<INetFwMgr>(create_instance<NetFwMgr, INetFwMgr>);
        auto policy = make_with_releaser<INetFwPolicy>(&INetFwMgr::get_LocalPolicy, *mgr);
        auto profile = make_with_releaser<INetFwProfile>(&INetFwPolicy::get_CurrentProfile, *policy);

        return profile;
    }

} // namespace flow::sysinfo::detail
