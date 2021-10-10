#pragma once

#include <functional>
#include <map>
#include <memory>

namespace CGCP {
    template<class T>
    class Solution {
        using tptr = std::unique_ptr<T>;
        using creator = std::function<tptr(void)>;

    private:
        std::map<std::string, tptr> items_;
        std::map<std::string, creator> creators_;

    public:
        Solution &add(std::string key, creator c) {
            creators_[key] = c;
            return *this;
        };

        T &get(std::string key) {
            if (items_.count(key) == 0)
                items_[key] = creators_[key]();

            return *items_[key];
        };

        virtual ~Solution() = default;
    };
}// namespace CGCP