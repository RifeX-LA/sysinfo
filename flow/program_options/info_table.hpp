#pragma once

#include <ranges>
#include <algorithm>
#include <unordered_map>
#include <boost/property_tree/json_parser.hpp>
#include <flow/program_options/info.hpp>

namespace flow::sysinfo {

    class info_table {
    public:
        using inner_type = std::unordered_map<std::string, info>;

        info_table() = default;
        explicit info_table(inner_type&& map): m_map(std::move(map)) {}

        void print() {
            std::ranges::for_each(m_map, &info::print, &inner_type::value_type::second);
        }

        bool apply(std::string_view arg, bool unset_required = true) {
            auto parsed_name = m_parse_arg_name(arg);
            auto it = m_map.find(parsed_name.first);

            if (it == m_map.end()) {
                return false;
            }

            if (parsed_name.second) {
                if (unset_required) {
                    std::ranges::for_each(m_map, &info::unset, &inner_type::value_type::second);
                }
                it->second.set();
            }
            else {
                it->second.unset();
            }

            return parsed_name.second && unset_required;
        }

        void write_json(const std::filesystem::path& path) const {
            boost::property_tree::ptree root;
            auto printable_info = std::views::values(m_map) | std::views::filter(&info::flag);
            std::ranges::for_each(printable_info, [&](auto&& info){root.push_back(info.json().front());});

            std::ofstream fout(path);
            boost::property_tree::write_json(fout, root);
        }

    private:
        inner_type m_map;

        static std::pair<std::string, bool> m_parse_arg_name(std::string_view arg) {
            if (arg.starts_with("no-")) {
                return {arg.substr(3).data(), false};
            }
            return {arg.data(), true};
        }
    };

} // namespace flow::sysinfo
