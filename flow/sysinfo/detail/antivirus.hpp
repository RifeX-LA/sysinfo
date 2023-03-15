#pragma once

#include <string>
#include <iwscapi.h>

namespace flow::sysinfo::detail {

    std::string product_state_to_str(WSC_SECURITY_PRODUCT_STATE state) {
        switch (state) {
            case WSC_SECURITY_PRODUCT_STATE_ON:
                return "ON";
            case WSC_SECURITY_PRODUCT_STATE_OFF:
                return "OFF";
            case WSC_SECURITY_PRODUCT_STATE_SNOOZED:
                return "SNOOZED";
            default:
                return "EXPIRED";
        }
    }

    std::string signature_status_to_str(WSC_SECURITY_SIGNATURE_STATUS status) {
        switch (status) {
            case WSC_SECURITY_PRODUCT_UP_TO_DATE:
                return "UP TO DATE";
            default:
                return "OUT OF DATE";
        }
    }

} // namespace flow::sysinfo::detail
