/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    CRTP.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    01.07.2021 
 */ 
 
#ifndef CRTP_H_511914579211142519931711304181696459318881 
#define CRTP_H_511914579211142519931711304181696459318881 
 

namespace cpptools {


/**
 * @class CRTP
 * @brief CRTP is helper class that simplifies usage of CRTP template classes.
 */
template <typename Host, template <typename, typename...> class CRTPType,
          typename... AdditionalArgs>
struct CRTP {

  // ---------------------------------------------------
  // convenience methods
  auto asHost () -> Host&;
  auto asHost () const -> Host const&;

private:

  CRTP() noexcept = default;
  friend CRTPType<Host, AdditionalArgs...>;
};


/**
 * @brief returns a reference to the actual CRTP object.
 */
template <typename Host, template <typename, typename...> class CRTPType,
          typename... AdditionalArgs>
inline auto CRTP<Host, CRTPType, AdditionalArgs...>::asHost() -> Host&
{
  return static_cast<Host&>(*this);
}

/**
 * @brief returns a const reference to the actual CRTP object.
 */
template <typename Host, template <typename, typename...> class CRTPType,
          typename... AdditionalArgs>
inline auto CRTP<Host, CRTPType, AdditionalArgs...>::asHost() const -> Host const&
{
  return static_cast<Host const&>(*this);
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // CRTP_H_511914579211142519931711304181696459318881 
