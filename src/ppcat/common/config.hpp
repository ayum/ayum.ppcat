
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <tuple>

namespace ppcat::common {

template<typename ...T>
struct config {
    template<typename C>
    C &get() {
        return std::get<C>(_config);
    }

private:
    std::tuple<T...> _config;
};

}

#endif /* CONFIG_HPP */
