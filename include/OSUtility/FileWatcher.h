/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FileWatcher.h 
 * @brief   FileWatcher is a simple hotfolder class that can observe several
 *          folders at once.
 * 
 * @author  Lasse Rosenthal 
 * @date    24.08.2021 
 */ 
 
#ifndef FILEWATCHER_H_160963211423772179061091215091134022270719178 
#define FILEWATCHER_H_160963211423772179061091215091134022270719178 
 
 
// includes
#include <ConcurrencyTools/HashMap.h>
#include <ConcurrencyTools/RAIIThread.h>

#include <chrono>
#include <filesystem>
#include <functional>
#include <list>
#include <unordered_map>


namespace osutil {


/** 
 * @class FileWatcher
 * @brief FileWatcher is a simple hotfolder class that can observe several
 *        hotfolders at once.
 */
class FileWatcher {

public:

  /// simple enumeration to describe the state of a file
  enum class fileStatus {
    created,
    modified,
    erased
  };

  // ---------------------------------------------------
  // public types
  using path      = std::filesystem::path;
  using size_type = std::size_t;
  using callback  = std::function<void(path const&, fileStatus)>;

  // ---------------------------------------------------
  // special member functions
  FileWatcher    (std::chrono::milliseconds refreshTime, callback, std::list<path> const& folders = {});
  FileWatcher    (std::chrono::milliseconds refreshTime, std::list<path> const& folders = {});
  FileWatcher    (FileWatcher const&) = delete;
  FileWatcher    (FileWatcher&&) = default;
  auto operator= (FileWatcher const&) -> FileWatcher & = delete;
  auto operator= (FileWatcher&&) -> FileWatcher & = default;
  ~FileWatcher   ();

  // ---------------------------------------------------
  // api
  auto start        (callback newNotifier) -> bool;
  void stop         () noexcept;
  auto isRunning    () const noexcept -> bool;
  auto size         () const noexcept -> size_type;
  void addFolder    (path const& folder);
  auto removeFolder (path const& folder) -> bool;

private:

  // ---------------------------------------------------
  // private types
  using FileModificationTimes    = std::unordered_map<std::wstring, std::filesystem::file_time_type>;
  using FileModificationTimesMap = cctools::ThreadsafeHashMap<std::wstring, FileModificationTimes>;

  // ---------------------------------------------------
  // private data
  std::chrono::milliseconds refreshTime;
  callback                  notifier;
  bool                      isActive;
  FileModificationTimesMap  fileModificationTimesMap;
  cctools::JoinThread       watchThread;

  // ---------------------------------------------------
  // private methods
  void loop               ();
  void checkFolders       ();
  void checkFiles         (path const& folder, FileModificationTimes& fileModificationTimes);
  void testForErasedFiles (FileModificationTimes& fileModificationTimes);
};


/**
 * @brief Constructor. Registers a given callback and starts the folder watch thread.
 * @param notifier the notification callback which is invoked upon creation, modification
 *        or deletion of file.
 * @param folders a list of folders that are observed by the FileWatcher.
 */
inline FileWatcher::FileWatcher(std::chrono::milliseconds refreshTime, callback notifier,
                                std::list<path> const& folders)
  : refreshTime {refreshTime}
  , notifier    {std::move(notifier)}
  , isActive    {true}
{
  for(const auto& folder : folders)
  {
    addFolder(folder);
  }

  watchThread = cctools::JoinThread{[this] { loop(); }};
}

/**
 * @brief  Constructor. Registers a list of folders to be observed.
 * @param  folders a list of folders that are observed by the FileWatcher.
 * @remark This constructor does not start the file watching thread.
 */
inline FileWatcher::FileWatcher(std::chrono::milliseconds refreshTime, std::list<path> const& folders)
  : refreshTime {refreshTime}
  , isActive    {false}
{
  for(const auto& folder : folders)
  {
    addFolder(folder);
  }
}

/**
 * @brief Destructor. Stops the watching thread.
 */
inline FileWatcher::~FileWatcher()
{
  stop();
}

/**
 * @brief stops the folder watch loop.
 */
inline void FileWatcher::stop() noexcept
{
  isActive = false;
}

/**
 * @brief Checks if the FileWatcher is running.
 */
inline auto FileWatcher::isRunning() const noexcept -> bool
{
  return isActive;
}

/**
 * @brief Returns the number of observed folders.
 */
inline auto FileWatcher::size() const noexcept -> size_type
{
  return fileModificationTimesMap.size();
}

/**
 * @brief Adds a given folder to the FileWatcher.
 */
inline void FileWatcher::addFolder(path const& folder)
{
  return fileModificationTimesMap.insertOrReplace(folder, FileModificationTimes{});
}

/**
 * @brief  removes a given folder from the FileWatcher.
 * @return returns true if the folder was successfully removed.
 */
inline auto FileWatcher::removeFolder(path const& folder) -> bool
{
  return fileModificationTimesMap.erase(folder.wstring()) != 0ULL;
}


}   // namespace osutil


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FILEWATCHER_H_160963211423772179061091215091134022270719178 
