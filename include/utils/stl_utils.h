
#ifndef UTILS_STL_UTILS_H
#define UTILS_STL_UTILS_H

namespace utils
{
template <typename Cont>
inline void swap_erase(Cont& c, typename Cont::iterator it)
{
    std::swap(*it, c.back());
    c.pop_back();
}
} // namespace utils

#endif
