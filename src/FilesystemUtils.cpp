/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FilesystemUtils.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    06.05.2021 
 */ 
 
// includes 
#include <Utils/FilesystemUtils.h>


namespace fsutils {


// deleteDirectoryFiles
void deleteDirectoryFiles(std::filesystem::path const& dir)
{
  for(auto& p : std::filesystem::directory_iterator(dir))
  {
    if(std::filesystem::is_regular_file(p))
    {
      std::filesystem::remove(p);
    }
  }
}


}   // namespace fsutils


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
