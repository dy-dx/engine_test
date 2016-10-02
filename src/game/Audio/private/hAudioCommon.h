
#pragma once

#include <SDL_assert.h>

template<typename T>
auto ToImpl(T* thisptr) -> typename T::Impl*
{
    auto ptr = static_cast<typename T::Impl*>(thisptr);
    SDL_assert(ptr);
    return ptr;
}

template<typename T>
auto ToImpl(const T* thisptr) -> typename const T::Impl*
{
    auto ptr = static_cast<typename const T::Impl*>(thisptr);
    SDL_assert(ptr);
    return ptr;
}