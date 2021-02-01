/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesString.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.02.2021
 */ 
 
#ifndef XERCESSTRING_H_12477152691811307958786671621605725712 
#define XERCESSTRING_H_12477152691811307958786671621605725712 
 
 
// includes
#include <XercesUtils/XercesTranscoder.h>

#include <memory>
#include <ostream>
#include <string>
#include <utility>


namespace xcu {
 
 
/** 
 * @class XercesString
 * @brief 
 */
class XercesString {

public:

  // ---------------------------------------------------
  // public types
  using value_type = ::XMLCh;
  using size_type  = std::size_t;

  // ---------------------------------------------------
  // special member functions
  XercesString          () = default;
  XercesString          (XercesString const& src);
  XercesString          (XercesString&& src) = default;
  auto operator=        (XercesString const& src) -> XercesString&;
  auto operator=        (XercesString&& src) -> XercesString& = default;
  explicit XercesString (std::string const& s);
  explicit XercesString (const char* s);
  template <std::enable_if_t<wcharEnabled, void**> = nullptr>
  explicit XercesString (std::wstring const& s);
  template <std::enable_if_t<wcharEnabled, void**> = nullptr>
  explicit XercesString (const wchar_t* s);
  ~XercesString         () = default;

  // ---------------------------------------------------
  // public methods
  auto size     () const noexcept -> size_type;
  auto empty    () const noexcept -> bool;
  auto isValid  () const noexcept -> bool;
  void swap     (XercesString& other) noexcept;
  auto data     () const noexcept -> value_type*;
  auto append   (XercesString const& src) -> XercesString&;
  template <typename StrT>
  auto toString () const -> StrT;
  template <typename CharT, typename CharTraits = std::char_traits<CharT>>
  auto put      (std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&;


private:

  /**
   * @brief XercesRelease wraps the necessary calls of xercesc::XMLString::release
   *        for char* and ::XMLCh*
   */
  template <typename CharT>
  struct XercesRelease {
    void operator()(CharT* p) const
    {
      if(p != nullptr)
      {
        xercesc::XMLString::release(&p);
      }
    }
  };

  /**
   * @brief template specialization wchar_t.
   */
  template <>
  struct XercesRelease<wchar_t> {
    void operator()(wchar_t* p) const {
      if(p != nullptr)
      {
        delete[](p);
      }
    }
  };

  // ---------------------------------------------------
  // private types
  template <typename CharT>
  using XMLStringPtr = std::unique_ptr<CharT, XercesRelease<CharT>>;

  // ---------------------------------------------------
  // private data
  XMLStringPtr<value_type> strData;
  size_type                strLength{0ULL};

  // ---------------------------------------------------
  // private methods
  template <typename CharT>
  auto toRawString() const -> XMLStringPtr<CharT>;
};


/**
 * @brief copy constructor. Performs a deep copy.
 */
inline XercesString::XercesString(XercesString const& src)
  : strData   {xercesc::XMLString::replicate(src.strData.get())}
  , strLength {src.strLength}
{}

/**
 * @brief copy assignment.
 */
inline auto XercesString::operator=(XercesString const& src) -> XercesString&
{
  if(this != &src)
  {
    XercesString tmp(src);
    swap(tmp);
  }
  return *this;
}

/**
 * @brief constructs a XercesString from a std::string.
 */
inline XercesString::XercesString(std::string const& s)
  : strData   {XercesTranscoder<char>::toXMLCh(s.data()), XercesRelease<value_type>()}
  , strLength {s.length()}
{}

/**
 * @brief constructs a XercesString from a const char*.
 */
inline XercesString::XercesString(const char* s)
  : strData   {XercesTranscoder<char>::toXMLCh(s), XercesRelease<value_type>()}
  , strLength {std::char_traits<char>::length(s)}
{}

/**
 * @brief constructs a XercesString from a std::wstring.
 */
template <std::enable_if_t<wcharEnabled, void**>>
inline XercesString::XercesString(std::wstring const& s)
  : strData   {XercesTranscoder<wchar_t>::toXMLCh(s.data()), XercesRelease<wchar_t>()}
  , strLength {s.length()}
{}

/**
 * @brief constructs a XercesString from a const wchar_t*.
 */
template <std::enable_if_t<wcharEnabled, void**>>
inline XercesString::XercesString(const wchar_t* s)
  : strData   {XercesTranscoder<wchar_t>::toXMLCh(s), XercesRelease<wchar_t>()}
  , strLength {std::char_traits<wchar_t>::length(s)}
{}

/**
 * @brief Returns the number of characters in the string.
 */
inline auto XercesString::size() const noexcept -> size_type
{
  return strLength;
}

/**
 * @brief Checks if the string is empty.
 */
inline auto XercesString::empty() const noexcept -> bool
{
  return strLength == 0ULL;
}

/**
 * @brief Checks if the string is pointing to a valid adress.
 */
inline auto XercesString::isValid() const noexcept -> bool
{
  return !!strData;
}

/**
 * @brief Exchanges the contents of the string with the contents of other
 */
inline void XercesString::swap(XercesString& other) noexcept
{
  std::swap(strLength, other.strLength);
  std::swap(strData, other.strData);
}

/**
 * @brief Returns a pointer to the first character of tthe string.
 */
inline auto XercesString::data() const noexcept -> value_type*
{
  return strData.get();
}

/**
 * @brief Converts to s given string type.
 */
template <typename StrT>
inline auto XercesString::toString() const -> StrT
{
  if(isValid() && !empty())
  {
    const auto s = toRawString<typename StrT::value_type>();
    return StrT{s.get()};
  }

  return {};
}

/**
 * @brief Converts and copies the string content into a std::uniqu_ptr of a given character type.
 */
template <typename CharT>
auto XercesString::toRawString() const -> XMLStringPtr<CharT>
{
  if(isValid())
  {
    return XMLStringPtr<CharT>{XercesTranscoder<CharT>::toChar(strData.get())};
  }

  return {};  
}

/**
 * @brief Puts the content of the string into a given std::basic_ostream
 */
template <typename CharT, typename CharTraits>
auto XercesString::put(std::basic_ostream<CharT, CharTraits>& ostr) const -> std::basic_ostream<CharT, CharTraits>&
{
  if(isValid())
  {
    return ostr << toRawString<CharT>().get();
  }

  return ostr;
}


/**
 * @brief Concatenates two XercesString objects.
 */
inline auto operator+(XercesString const& XMLStr1, XercesString const& XMLStr2) -> XercesString
{
  XercesString tmp(XMLStr1);
  return tmp.append(XMLStr2);
}

/**
 * @brief Streaming operator.
 */
template <typename CharT, typename CharTraits>
inline auto operator<<(std::basic_ostream<CharT, CharTraits>& ostr, XercesString const& XMLStr) -> std::basic_ostream<CharT, CharTraits>&
{
  return XMLStr.put(ostr);
}



namespace literals {

inline auto operator"" _xs(const char* s, std::size_t) -> XercesString { return XercesString(s);}
inline auto operator"" _xs(const wchar_t* s, std::size_t) -> XercesString { return XercesString(s);}

}

}   // namespace xcu


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // XERCESSTRING_H_12477152691811307958786671621605725712 
