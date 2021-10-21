/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StringJoiner.h 
 * @brief   StringJoiner is a small class that allows concatenating of
 *          string with static storage at compile time.
 * 
 * @author  Lasse Rosenthal 
 * @date    10.09.2021 
 */ 
 
#ifndef STRINGJOINER_H_85994570147122972381113140016400242138635 
#define STRINGJOINER_H_85994570147122972381113140016400242138635 
 
 
// includes
#include <array>
#include <string_view>
 
 
namespace meta {


/** 
 * @struct StringJoiner 
 * @brief  StringJoiner is a small class that allows concatenating of
 *         string with static storage at compile time.
 * @tparam CharT the value_type of the resulting string.
 * @tparam Strings a non-type parameter pack of type reference to
 *         std::basic_string_view<CharT>.
 */
template <typename CharT, std::basic_string_view<CharT> const&... Strings>
struct StringJoinerT {

private:

  // ---------------------------------------------------
  // privte helpers
  static constexpr auto join() noexcept;
  static constexpr auto stringArray = join();

public:

  // ---------------------------------------------------
  // the joined string
  static constexpr std::basic_string_view<CharT> value {stringArray.data(), stringArray.size() - 1ULL};
};

/** 
 * @brief creates a std::array that holds the concatenation of all strings
 *        in the parameter pack.
 */
template <typename CharT, std::basic_string_view<CharT> const&... Strings>
constexpr auto StringJoinerT<CharT, Strings...>::join() noexcept
{
  constexpr auto length = (Strings.size() + ... + 0ULL);
  std::array<CharT, length + 1ULL> str{};

  auto append = [i = 0ULL, &str](auto const& s) mutable {
    for(auto const c : s)
    {
      str[i] = c;
      ++i;
    }
  };
  (append(Strings), ...);
  str[length] = CharT('\0');

  return str;
}

/** 
 * @brief Convenient variable template deducing the character type parameter of the
 *        concatenation automatically.
 */
template <auto const& String, auto const&... Strings>
constexpr auto JoinedString = StringJoinerT<std::remove_reference_t<decltype(String)>::value_type, String, Strings...>::value;


}   // namespace meta


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STRINGJOINER_H_85994570147122972381113140016400242138635 
