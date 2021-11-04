#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace CGCP {
    template<class T>
    class Solution {
        using tptr = std::shared_ptr<T>;
        using creator = std::function<tptr(void)>;

    private:
        std::map<std::string, tptr> items_;
        std::map<std::string, creator> creators_;

    public:
        Solution() = default;
        virtual ~Solution() = default;

        Solution &add(std::string key, creator c) {
            creators_[key] = c;
            return *this;
        };

        std::shared_ptr<T> get(std::string key) {
            if (items_.count(key) == 0)
                items_[key] = creators_[key]();

            return items_[key];
        };

        std::vector<std::string> getKeys() {
            auto keys = std::vector<std::string>();
            for (const auto &[key, value] : creators_)
                keys.push_back(key);
            return keys;
        }
    };
}// namespace CGCP
