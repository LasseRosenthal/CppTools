/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FileWatcher.cpp 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    24.08.2021 
 */ 
 
// includes 
#include <OSUtility/FileWatcher.h>


namespace fs = std::filesystem;


namespace osutil {


/**
 * @brief The main loop which iterates through all observed directories and notifiees the client
 *        via the registered callback method if any changes have been found.
 */
void FileWatcher::loop()
{
  while(isActive)
  {
    std::this_thread::sleep_for(refreshTime);
  
    fileModificationTimesMap.forEachValue([this](auto& folderAndfileModificationTimesMap) {
      checkFiles(folderAndfileModificationTimesMap.first, folderAndfileModificationTimesMap.second);
    });
  }
}

/**
 * @brief Iterates through all observed directories and checks if any files files have been changed.
 */
void FileWatcher::checkFiles(path const& folder, FileModificationTimes& fileModificationTimes)
{
  testForErasedFiles(fileModificationTimes);

  for(auto& dirEntry : fs::recursive_directory_iterator{folder})
  {
    const auto filePath    = dirEntry.path().wstring();
    const auto lastModTime = fs::last_write_time(dirEntry.path());

    if(fileModificationTimes.find(filePath) == fileModificationTimes.end())
    {
      fileModificationTimes[filePath] = lastModTime;
      notifier(path{filePath}, fileStatus::created);
    }
    else if(fileModificationTimes[filePath] != lastModTime)
    {
      fileModificationTimes[filePath] = lastModTime;
      notifier(path{filePath}, fileStatus::modified);
    }
  }
}

/**
 * @brief Tests if any file has been deleted in a given directory.
 */
void FileWatcher::testForErasedFiles(FileModificationTimes& fileModificationTimes)
{
  auto fileIt = fileModificationTimes.begin();
  while(fileIt != fileModificationTimes.end())
  {
    std::error_code errorCode;
    if(const path file{fileIt->first}; !fs::exists(file, errorCode))
    {
      notifier(file, fileStatus::erased);
      fileIt = fileModificationTimes.erase(fileIt);
    }
    else
    {
      ++fileIt;
    }
  }
}

/**
 * @brief  Starts the folder watch thread with a given notification callback.
 *         If the FileWatcher is already associated with a folder watch thread
 *         this method simply returns false.
 * @return returns true if the FileWatcher has successfully been started with the given
 *         callback method.
 */
auto FileWatcher::start(callback newNotifier) -> bool
{
  if(isActive)
  {
    return false;
  }

  notifier    = std::move(newNotifier);
  isActive    = true;
  watchThread = cctools::JoinThread{[this] { loop(); }};

  return true;
}


}   // namespace osutil


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
