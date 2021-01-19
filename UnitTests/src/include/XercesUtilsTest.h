/* -------------------------------------------------------------------------- 
 * Copyright (c) 2021 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    XercesUtilsTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    19.01.2021 
 */ 
 
#ifndef XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
#define XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
 
 
// includes 
#include <XercesUtils/XercesTranscoder.h>
 
 
TEST(XercesUtils, wcharEnabled)
{
  EXPECT_TRUE(xercesutils::wcharEnabled);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // XERCESUTILSTEST_H_3000518017122002883215805212592230343766657 
