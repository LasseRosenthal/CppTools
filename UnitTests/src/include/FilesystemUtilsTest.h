/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    FilesystemUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    13.01.2021 
 */ 
 
#ifndef FILESYSTEMUTILSTEST_H_20891381413781850214402315516808130294 
#define FILESYSTEMUTILSTEST_H_20891381413781850214402315516808130294 
 
 
// includes
#include <Utils/FilesystemUtils.h>

#include <fstream>


using namespace std::string_literals;

namespace fs = std::filesystem;
 
TEST(FilesystemUtils, extendFileNameNoParentPathNoExtension)
{
  const std::filesystem::path p{LR"(test)"};
  const std::filesystem::path expected{LR"(test_bkp)"};

  const auto p1 = cpptools::addToFilename(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}

TEST(FilesystemUtils, extendFileNameNoParentPath)
{
  const std::filesystem::path p{LR"(user.config.xml)"};
  const std::filesystem::path expected{LR"(user.config_bkp.xml)"};

  const auto p1 = cpptools::addToFilename(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}

TEST(FilesystemUtils, extendFileNameIncludingParentPath)
{
  const std::filesystem::path p{LR"(C:\Users\prinect\Documents\user.config.xml)"};
  const std::filesystem::path expected{LR"(C:\Users\prinect\Documents\user.config_bkp.xml)"};

  const auto p1 = cpptools::addToFilename(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}

TEST(FilesystemUtils, deleteDirectoryContent)
{
  const std::filesystem::path currentDir = fs::current_path();

  const auto testDir = currentDir / "Test";
  const auto file         = testDir / "datei.txt";

  fs::create_directories(testDir);

  std::ofstream(file.string());

  cpptools::deleteDirectoryContent(testDir);

  fs::remove_all(testDir);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FILESYSTEMUTILSTEST_H_20891381413781850214402315516808130294 
