//
// Created by retard on 10.10.22.
//

#ifndef OS_LABS_UTILS_H
#define OS_LABS_UTILS_H

template <typename Container>
inline unsigned int isize(const Container &c)
{
    return static_cast<unsigned int>(c.size());
}

#endif // OS_LABS_UTILS_H
