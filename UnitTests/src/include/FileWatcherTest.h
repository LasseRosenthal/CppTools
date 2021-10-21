/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FileWatcherTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    24.08.2021 
 */ 
 
#ifndef FILEWATCHERTEST_H_255212548231781178732317468525874174793753 
#define FILEWATCHERTEST_H_255212548231781178732317468525874174793753 
 
 
// includes
#include <OSUtility/FileWatcher.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;
using namespace std::chrono_literals;
using namespace std::string_literals;


class FileWatcherTest : public ::testing::Test {

protected:

  // ---------------------------------------------------
  // methods inherited from ::testing::Test
  void SetUp    () override;
  void TearDown () override;

  std::chrono::milliseconds refreshTime{80ms};

  std::size_t           numHotFolders{4};
  std::vector<fs::path> hotFolders;
  std::size_t           numTestFiles{3};
  std::vector<fs::path> testFiles;
  bool hotFolderCreated{false};
};

void FileWatcherTest::SetUp()
{
  hotFolders.reserve(numHotFolders);
  std::stringstream hotFolder;
  for(std::size_t i = 0ULL; i < numHotFolders; ++i)
  {
    hotFolder.str("");
    hotFolder << "myHotFolder_" << i;
    hotFolders.push_back(fs::current_path() / hotFolder.str());
  }

  testFiles.reserve(numTestFiles);
  std::stringstream testFile;
  for(std::size_t i = 0ULL; i < numTestFiles; ++i)
  {
    testFile.str("");
    testFile << (fs::current_path() / "test").string() << i << ".txt";
    testFiles.push_back(testFile.str());
  }

  for(const auto& f : testFiles)
  {
    std::ofstream fileStrm{f};
  }

  for(const auto& folder : hotFolders)
  {
    hotFolderCreated = fs::create_directory(folder);
  }
}

void FileWatcherTest::TearDown()
{
  if(hotFolderCreated)
  {
    for(const auto& folder : hotFolders)
    {
      fs::remove_all(folder);
    }
  }

  for(const auto& f : testFiles)
  {
    fs::remove(f);
  }
}

TEST_F(FileWatcherTest, FileCreationOneHotFolder)
{
  std::list<fs::path> myHotFolders{hotFolders[0]};

  std::stringstream hotFolderMessages;
  auto callback = [&hotFolderMessages](fs::path f, osutil::FileWatcher::fileStatus s) {
    if(s == osutil::FileWatcher::fileStatus::created)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file created : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::modified)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file modified : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::erased)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file erased : " << f.string();
    }
  };

  osutil::FileWatcher watcher(refreshTime, callback, myHotFolders);

  std::stringstream expectedMessage;

  for(const auto& hotFolder : myHotFolders)
  {
    for(const auto& testFile : testFiles)
    {
      fs::copy(testFile, hotFolder);

      std::this_thread::sleep_for(300ms);
      expectedMessage << "file created : " << (hotFolder / testFile.filename()).string();

      EXPECT_EQ(hotFolderMessages.str(), expectedMessage.str());
      expectedMessage.str("");
    }
  }

  EXPECT_TRUE(hotFolderCreated);
}

TEST_F(FileWatcherTest, FileCreationModificationAndDeletion)
{
  std::list<fs::path> myHotFolders{hotFolders[0], hotFolders[1]};

  std::this_thread::sleep_for(1s);

  std::stringstream hotFolderMessages;
  auto callback = [&hotFolderMessages](fs::path f, osutil::FileWatcher::fileStatus s) {
    if(s == osutil::FileWatcher::fileStatus::created)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file created : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::modified)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file modified : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::erased)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file erased : " << f.string();
    }
  };

  osutil::FileWatcher watcher(refreshTime, callback, myHotFolders);

  std::stringstream expectedMessage;

  for(const auto& hotFolder : myHotFolders)
  {
    for(const auto& testFile : testFiles)
    {
      fs::copy(testFile, hotFolder);
      std::this_thread::sleep_for(800ms);
      expectedMessage.str("");
      expectedMessage << "file created : " << (hotFolder / testFile.filename()).string();
      EXPECT_EQ(hotFolderMessages.str(), expectedMessage.str());
    }
  }

  for(const auto& hotFolder : myHotFolders)
  {
    for(const auto& testFile : testFiles)
    {
      {
        std::ofstream fileStrm(hotFolder / testFile.filename());
        if(fileStrm)
        {
          fileStrm << "this is a file\n";
          fileStrm.close();
        }
      }

      std::this_thread::sleep_for(400ms);
      expectedMessage.str("");
      expectedMessage << "file modified : " << (hotFolder / testFile.filename()).string();

      EXPECT_EQ(hotFolderMessages.str(), expectedMessage.str());
    }
  }

  for(const auto& hotFolder : myHotFolders)
  {
    for(const auto& testFile : testFiles)
    {
      const auto fileInHotFolder = hotFolder / testFile.filename();
      fs::remove(fileInHotFolder);

      std::this_thread::sleep_for(200ms);
      expectedMessage.str("");
      expectedMessage << "file erased : " << fileInHotFolder.string();

      EXPECT_EQ(hotFolderMessages.str(), expectedMessage.str());
    }
  }

  EXPECT_TRUE(hotFolderCreated);
}

TEST_F(FileWatcherTest, removeFolder)
{
  std::this_thread::sleep_for(1s);
  std::list<fs::path> myHotFolders{hotFolders[2], hotFolders[3]};

  std::stringstream hotFolderMessages;
  auto callback = [&hotFolderMessages](fs::path f, osutil::FileWatcher::fileStatus s) {
    if(s == osutil::FileWatcher::fileStatus::created)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file created : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::modified)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file modified : " << f.string();
    }
    if(s == osutil::FileWatcher::fileStatus::erased)
    {
      hotFolderMessages.str("");
      hotFolderMessages << "file erased : " << f.string();
    }
  };

  osutil::FileWatcher watcher(refreshTime);
  const bool started = watcher.start(callback);

  EXPECT_TRUE(started);
  EXPECT_EQ(watcher.size(), 0ULL);

  for(const auto& f : myHotFolders)
  {
    watcher.addFolder(f);
  }

  EXPECT_EQ(watcher.size(), 2ULL);

  std::stringstream expectedMessage;
  for(const auto& hotFolder : myHotFolders)
  {
    for(const auto& testFile : testFiles)
    {
      fs::copy(testFile, hotFolder);
      std::this_thread::sleep_for(800ms);
      expectedMessage.str("");
      expectedMessage << "file created : " << (hotFolder / testFile.filename()).string();
      EXPECT_EQ(hotFolderMessages.str(), expectedMessage.str());
    }
  }

  watcher.removeFolder(hotFolders[2]);
  EXPECT_EQ(watcher.size(), 1ULL);

  hotFolderMessages.str("");

  for(const auto& testFile : testFiles)
  {
    const auto fileInHotFolder = hotFolders[2] / testFile.filename();
    fs::remove(fileInHotFolder);

    EXPECT_EQ(hotFolderMessages.str(), ""s);
  }


  EXPECT_TRUE(hotFolderCreated);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FILEWATCHERTEST_H_255212548231781178732317468525874174793753 
