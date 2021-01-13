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
 
namespace fs = std::filesystem;

using namespace std::string_literals;
using namespace cpptools;
 

TEST(FilesystemUtils, extendFileNameNoParentPathNoExtension)
{
  const fs::path p{LR"(test)"};
  const fs::path expected{LR"(test_bkp)"};

  const auto p1 = extendFileName(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}
TEST(FilesystemUtils, extendFileNameNoParentPath)
{
  const fs::path p{LR"(user.config.xml)"};
  const fs::path expected{LR"(user.config_bkp.xml)"};

  const auto p1 = extendFileName(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}
TEST(FilesystemUtils, extendFileNameIncludingParentPath)
{
  const fs::path p{LR"(C:\Users\prinect\Documents\user.config.xml)"};
  const fs::path expected{LR"(C:\Users\prinect\Documents\user.config_bkp.xml)"};

  const auto p1 = extendFileName(p, L"_bkp"s);
  EXPECT_EQ(p1, expected);
}

 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // FILESYSTEMUTILSTEST_H_20891381413781850214402315516808130294 
