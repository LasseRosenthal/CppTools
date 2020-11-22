/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    ImpoSurfaceSheetRelationTest.h 
 * @brief 
 * 
 * @author  Lasse Rosenthal 
 * @date    03.11.2020 
 */ 
 
#ifndef IMPOSURFACESHEETRELATIONTEST_H_1901235973079110906160421171253362153117838 
#define IMPOSURFACESHEETRELATIONTEST_H_1901235973079110906160421171253362153117838 
 
 
// includes 
#include <ArbeitsCode/ImpoSurfaceSheetRelation.h>

class SurfaceSheetRelation : public ::testing::Test {

protected:

  // ---------------------------------------------------
  // methods inherited from ::testing::Test
  void SetUp() override;

  // ---------------------------------------------------
  // parameters prepared by this test fixture
  std::vector<int> allSheetPositions;
  std::vector<int> sheetRange;
  std::vector<int> tabSheets;
  int spreadType = 2;

  void setTotalNumberOfSheets(int n) { 
    allSheetPositions.resize(n);
    std::iota(allSheetPositions.begin(), allSheetPositions.end(), 1);
  }

  void setPrintSheetRange(int n1,  int n2) {
    int sum = n2 - n1 + 1;
    sheetRange.resize(sum);
    std::iota(sheetRange.begin(), sheetRange.end(), n1);
  }

  void setTabSheetRange(int n1, int delta, int sum) {
    tabSheets.resize(sum);
    std::generate(tabSheets.begin(), tabSheets.end(), [delta, n = n1]() mutable {
      int tmp = n;
      n += delta;
      return tmp;
    });
  }

};

inline void SurfaceSheetRelation::SetUp()
{}


TEST_F(SurfaceSheetRelation, surfacePositionsNoTabSheetsDuplex)
{
  const int totalNumSheets = 40;
  setTotalNumberOfSheets(totalNumSheets);

  const int n1 = 12;
  const int n2 = 19;
  setPrintSheetRange(n1, n2);

  const int numTabSheets = 0;
  const int first = 4;
  const int delta = 3;
  setTabSheetRange(first, delta, numTabSheets);

  UPCL::ImpoSurfaceSheetRelation sheetRelation(spreadType, sheetRange, allSheetPositions, tabSheets);
  const int numPrintSurfaces = sheetRelation.numPrintSurfaces();

  std::map<int, int> expectedPositions {
    {0, 12},
    {1, 12},
    {2, 13},
    {3, 13},
    {4, 14},
    {5, 14},
    {6, 15},
    {7, 15},
    {8, 16},
    {9, 16},
    {10, 17},
    {11, 17},
    {12, 18},
    {13, 18},
    {14, 19},
    {15, 19}
  };

  std::map<int, int> generatedPositions;
  for(int i = 0; i < numPrintSurfaces; ++i)
  {
    generatedPositions[i] = sheetRelation.getSheetPosAndId(i).first.first;
  }

  EXPECT_EQ(expectedPositions, generatedPositions);
}

TEST_F(SurfaceSheetRelation, surfacePositionsIncludingTabSheetsDuplex)
{
  const int totalNumSheets = 40;
  setTotalNumberOfSheets(totalNumSheets);

  const int n1 = 12;
  const int n2 = 19;
  setPrintSheetRange(n1, n2);

  const int numTabSheets = 40;
  const int first = 4;
  const int delta = 3;
  setTabSheetRange(first, delta, numTabSheets);

  UPCL::ImpoSurfaceSheetRelation sheetRelation(spreadType, sheetRange, allSheetPositions, tabSheets);
  const int numPrintSurfaces = sheetRelation.numPrintSurfaces();

  std::map<int, int> expectedPositions {
    {0, 12},
    {1, 12},
    {2, 13},
    {3, 14},
    {4, 14},
    {5, 15},
    {6, 15},
    {7, 16},
    {8, 17},
    {9, 17},
    {10, 18},
    {11, 18},
    {12, 19}
  };

  std::map<int, int> generatedPositions;
  for(int i = 0; i < numPrintSurfaces; ++i)
  {
    generatedPositions[i] = sheetRelation.getSheetPosAndId(i).first.first;
  }

  EXPECT_EQ(expectedPositions, generatedPositions);
}

TEST_F(SurfaceSheetRelation, surfacePositionsIncludingTabSheetsWiethDelta1Duplex)
{
  const int totalNumSheets = 40;
  setTotalNumberOfSheets(totalNumSheets);

  const int n1 = 12;
  const int n2 = 19;
  setPrintSheetRange(n1, n2);

  const int numTabSheets = 40;
  const int first = 4;
  const int delta = 1;
  setTabSheetRange(first, delta, numTabSheets);

  UPCL::ImpoSurfaceSheetRelation sheetRelation(spreadType, sheetRange, allSheetPositions, tabSheets);
  const int numPrintSurfaces = sheetRelation.numPrintSurfaces();

  std::map<int, int> expectedPositions {
    {0, 12},
    {1, 13},
    {2, 14},
    {3, 15},
    {4, 16},
    {5, 17},
    {6, 18},
    {7, 19}
  };

  std::map<int, int> generatedPositions;
  for(int i = 0; i < numPrintSurfaces; ++i)
  {
    generatedPositions[i] = sheetRelation.getSheetPosAndId(i).first.first;
  }

  EXPECT_EQ(expectedPositions, generatedPositions);
}
 
 
// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // IMPOSURFACESHEETRELATIONTEST_H_1901235973079110906160421171253362153117838 
