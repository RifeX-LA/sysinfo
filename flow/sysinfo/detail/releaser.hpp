#pragma once

#include <memory>
#include <WTypesbase.h>
#include <iostream>
#include <netfw.h>

namespace flow::sysinfo::detail {

    struct releaser {
        template <typename T>
        void operator()(T* ptr) const {
            ptr->Release();
        }
    };

    template <typename T, typename Fn, typename... Args>
    std::unique_ptr<T, releaser> make_with_releaser(Fn&& fn, Args&&... args) {
        T* ptr = nullptr;
        std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)..., std::addressof(ptr));
        return std::unique_ptr<T, releaser>(ptr);
    }

    template <typename U, typename T>
    void create_instance(T** ptr) {
        CoCreateInstance(__uuidof(U), nullptr, CLSCTX_INPROC_SERVER, __uuidof(T), reinterpret_cast<void**>(ptr));
    }

} // namespace flow::sysinfo::detail
