#pragma once

#include <utility>
#include <string>
#include <atlbase.h>
#include <wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

namespace flow::sysinfo::detail {

    std::pair<int, std::string> antivirus_info(const CComPtr<IWbemClassObject>& object) {
        CComVariant variant;
        object->Get(L"displayName", 0, &variant, nullptr, nullptr);
        std::string name(CW2A(variant.bstrVal));

        variant.Clear();
        object->Get(L"productState", 0, &variant, nullptr, nullptr);

        return {variant.intVal, std::move(name)};
    }

} // namespace flow::sysinfo::detail
