#pragma once

#include <string>
#include <wuapi.h>

namespace flow::sysinfo::detail {

    std::string download_mode_to_str(AutoDownloadMode mode) {
        switch (mode) {
            case adLetWindowsUpdateDecide:
                return "Let Windows update decide";
            case adNeverAutoDownload:
                return "Never auto-download";
            default:
                return "Always auto-download";
        }
    }

} // namespace flow::sysinfo::detail
