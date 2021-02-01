/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesTranscoder.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.01.2021 
 */ 
 
#ifndef XERCESTRANSCODER_H_28963299151516220347225613817124712067315108 
#define XERCESTRANSCODER_H_28963299151516220347225613817124712067315108 
 
 
// includes
#include <xercesc/util/XMLString.hpp>

#include <cwchar>
#include <type_traits>


namespace xcu {


// is XERCES_XMLCH_T defined as wchar_t?
constexpr bool wcharEnabled = std::is_same_v<::XMLCh, wchar_t>;


/**
 * @brief XercesTranscoder is a class template for transforming between 
 *        ::XMLCh* and the passed character type template parameter.
 */
template <typename CharT, bool IsChar = std::is_same_v<CharT, char>,
          bool IsWchar_t = std::is_same_v<CharT, wchar_t>, bool = true>
struct XercesTranscoder;


/**
 * @brief Partial specialization of XercesTranscoder for char.
 */
template <typename CharT>
struct XercesTranscoder<CharT, true, false, true> {
  using value_type = char;
  static auto toXMLCh (const value_type* c) -> ::XMLCh*;
  static auto toChar  (const ::XMLCh* c) -> value_type*;
};

/**
 * @brief  converts from const char* src to ::XMLCh*.
 * @remark The returned buffer is dynamically allocated and
 *         the client is responsible for deleting it via XMLString::release
 */
template <typename CharT>
inline auto XercesTranscoder<CharT, true, false, true>::toXMLCh(const value_type* c) -> ::XMLCh*
{
  return xercesc::XMLString::transcode(c);
}

/**
 * @brief  converts from ::XMLCh* src to char*.
 * @remark The returned buffer is dynamically allocated and
 *         the client is responsible for deleting it via XMLString::release 
 */
template <typename CharT>
inline auto XercesTranscoder<CharT, true, false, true>::toChar(const ::XMLCh* c) -> value_type*
{
  return xercesc::XMLString::transcode(c);
}


/**
 * @brief  Partial specialization of XercesTranscoder for wchar_t.
 * @remark This specialization is only enabled if XMLCh is an alias for wchar_t.
 */
template <typename CharT>
struct XercesTranscoder<CharT, false, true, wcharEnabled> {
  using value_type = wchar_t;
  static auto toXMLCh(const value_type* src) -> ::XMLCh*;
  static auto toChar(const ::XMLCh* src) -> value_type*;
};

/**
 * @brief  converts from const wchar_t* src to ::XMLCh*.
 * @remark The returned buffer is dynamically allocated and
 *         the client is responsible for deleting it via delete[]. 
 */
template <typename CharT>
inline auto XercesTranscoder<CharT, false, true, wcharEnabled>::toXMLCh(const wchar_t* src) -> ::XMLCh*
{
  const auto length = std::wcslen(src) + 1ULL;
  ::XMLCh* dest = new ::XMLCh[length];
  wcscpy_s(dest, length, src);
  return dest;
}

/**
 * @brief  converts from const ::XMLCh* src to wchar_t*.
 * @remark The returned buffer is dynamically allocated and
 *         the client is responsible for deleting it via delete[].
 */
template <typename CharT>
inline auto XercesTranscoder<CharT, false, true, wcharEnabled>::toChar(const ::XMLCh* src) -> wchar_t*
{
  const auto length = std::wcslen(src) + 1ULL;
  auto* dest = new wchar_t[length];
  wcscpy_s(dest, length, src);
  return dest;
}


}   // namespace xcu


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // XERCESTRANSCODER_H_28963299151516220347225613817124712067315108 
