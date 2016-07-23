#pragma once

#include <utility>
#include <type_traits>
#include <SDL.h>
#include "exception.h"

namespace svge {
namespace core {

template<class Func, class... Args, class ReturnType = std::result_of_t<Func(Args...)>>
typename std::enable_if<!std::is_pointer<ReturnType>::value, ReturnType>::type
sdl_checked_call(Func&& func, Args&&... args) {
  ReturnType result = func(std::forward<Args>(args)...);
  if (result) {
    throw SDLCallFailed(std::string("SDL_GetError: \"") + SDL_GetError() + "\"");
  }
  return result;
}

template<class Func, class... Args, class ReturnType = std::result_of_t<Func(Args...)>>
typename std::enable_if<std::is_pointer<ReturnType>::value, ReturnType>::type
sdl_checked_call(Func&& func, Args&&... args) {
  ReturnType result = func(std::forward<Args>(args)...);
  if (!result) {
    throw SDLCallFailed(std::string("SDL_GetError: \"") + SDL_GetError() + "\"");
  }
  return result;
}

} // namespace core
} // namespace svge
