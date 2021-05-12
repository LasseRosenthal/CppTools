/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FilesystemUtils.h 
 * @brief   a loose collection of convenient methods to facilitate
 *          some common use cases of std::filesystem.
 * 
 * @author  Lasse Rosenthal 
 * @date    13.01.2021 
 */ 
 
#ifndef FILESYSTEMUTILS_H_266832694821727210211372926375138123149217593 
#define FILESYSTEMUTILS_H_266832694821727210211372926375138123149217593 
 
 
// includes
#include <filesystem>


namespace cpptools {


/**
 * @brief Extends the filename component without extension of a given path by a given string.
 */
template <typename StrT>
auto addToFilename(std::filesystem::path const& p, StrT const& ext) -> std::filesystem::path
{
  std::filesystem::path extFileName = p.parent_path() / p.stem();
  extFileName += ext;
  extFileName += p.extension();

  return extFileName;
}

/**
 * @brief Deletes all files and subfolders of a given directory.
 */
void deleteDirectoryContent(std::filesystem::path const& dir)
{
  for(auto& p : std::filesystem::directory_iterator(dir))
  {
    if(std::filesystem::is_regular_file(p))
    {
      std::filesystem::remove(p);
    }
  }
}


}   // namespace cpptools


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FILESYSTEMUTILS_H_266832694821727210211372926375138123149217593 
