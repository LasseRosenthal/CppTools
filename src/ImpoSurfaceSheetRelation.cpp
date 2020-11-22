//  ###################################################################
//
//  COPYRIGHT 2004 - 2018 Heidelberger Druckmaschinen AG
//       ALL RIGHTS RESERVED
//
//  SOURCE FILE: ImpoSurfaceSheetRelation.cpp
//
//  FILE DESCRIPTION:
//     Implementation of the ImpoSurfaceSheetRelation class
//
//  ###################################################################


// --------------------------------------------------------------------
//                             INCLUDES
// --------------------------------------------------------------------
#include <ArbeitsCode/ImpoSurfaceSheetRelation.h>

#include <algorithm>
#include <iterator>
#include <functional>
#include <set>
#include <utility>


namespace UPCL {

#if 0
auto ImpoSurfaceSheetRelation::getSheetPrintRange(PrRangeVECTOR const& surfacePrintRanges) const -> PrRangeVECTOR
{
  DBG_METHOD(ImpoSurfaceSheetRelation::getSheetPrintRange);

  if(surfacePrintRanges.empty())
  {
    return PrRangeVECTOR{};
  }

  std::set<int> sheets;
  try
  {
    for(const auto& printRange : surfacePrintRanges)
    {
      for(auto i = printRange.iFirst; i <= printRange.iLast; ++i)
      {
        sheets.insert(m_surfNumToSheetPos.at(i));
      }
    }
  }
  catch(std::exception const& ex)
  {
    DBG_ERROR("invalid surface number. [Msg : " << ex.what() << "]");
  }

  PrRangeVECTOR sheetRanges;
  int lastSheetpos = *(sheets.begin());
  sheetRanges.emplace_back(lastSheetpos);
  for(const auto sheetPos : sheets)
  {
    if(sheetPos - lastSheetpos > 1)
    {
      sheetRanges.emplace_back(sheetPos);
    }
    else
    {
      sheetRanges.back().iLast = sheetPos;
    }
    lastSheetpos = sheetPos;
  }

  return sheetRanges;
}
#endif

void ImpoSurfaceSheetRelation::initialize()
{
  // create the mapping of surface indices (starting with zero) to the positions of all sheets.
  createSurfaceNumbering(m_allSheetPositions, m_surfaceIndexToAllSheetPositions, m_biasAllSurfaceIndexing);

  // create the mapping of surface numbers to sheet positions.
  createSurfaceNumbering(m_printSheetPositions, m_surfaceIndexToPrintSheetPosition, m_biasPrintSurfaceIndexing);
  m_numPrintSurfaces = static_cast<int>(m_surfaceIndexToPrintSheetPosition.size());

  reduceMappingToPrintRange();
  createSurfaceNumToIdMapping();
}

void ImpoSurfaceSheetRelation::createSurfaceNumbering(IntVec& sheetRange, SheetPosMap& surfaceToSheetMap, int bias)
{
  // restrict the tab sheet range to the borders of the given sheet range.
  // sheetRange may contains positions from 1 < n1 < n2 < N.

  // get the number of the first print sheet.
  const auto firstPrintSheet = sheetRange.front();

  // and an an iterator to the number of the first tab sheet within the sheet range.
  const auto firstTabSheetInRange = std::find_if(m_tabSheetPositions.begin(), m_tabSheetPositions.end(),
                                                 [firstPrintSheet](int tabPos) { return tabPos >= firstPrintSheet; });

  // get the number of the last print sheet.
  const auto lastPrintSheet = sheetRange.back();
  const auto firstTabSheetOutOfRange = std::find_if(firstTabSheetInRange, m_tabSheetPositions.end(),
                                                    [lastPrintSheet](int tabPos) { return tabPos > lastPrintSheet; });

  // printSheetsAndNumSurfaces contains pairs mapping the printing sheet position to the number of surfaces on the sheet.
  const auto printSheetsAndNumSurfaces =
    mapSheetpositionsToNumberOfSurfaces(sheetRange.begin(), sheetRange.end(), firstTabSheetInRange, firstTabSheetOutOfRange);

  mapConsecutiveNumberingToSheetPositions(printSheetsAndNumSurfaces, surfaceToSheetMap, bias);
}

/**
 * @brief  creates a mapping of an ascending sequence of numbers starting with bias to sheet positions.
 * @remark the sheets position is the number of the sheet and an enum indicating front or back.
 */
void ImpoSurfaceSheetRelation::mapConsecutiveNumberingToSheetPositions(IntPairVec const& printSheetsAndNumSurfaces,
                                                                       SheetPosMap& surfaceToSheetMap, int bias)
{
  auto ctrToSheetPos = [](int c) { return 0 == c ? sheetSide::front : sheetSide::back; };
  for(const auto& posAndNumSurfaces : printSheetsAndNumSurfaces)
  {
    for(int i = 0; i < posAndNumSurfaces.second; ++i)
    {
      surfaceToSheetMap.emplace_hint(surfaceToSheetMap.end(), std::piecewise_construct,
                                     std::forward_as_tuple(bias),
                                     std::forward_as_tuple(std::make_pair(posAndNumSurfaces.first, ctrToSheetPos(i))));
      ++bias;
    }
  }
}

/**
  * @brief  removes all sheets positions from the surfaceId to sheets positions map that are outside the print range.
  */
void ImpoSurfaceSheetRelation::reduceMappingToPrintRange()
{
  auto makePairCompare = [](SheetPosMap::value_type const& p1) -> std::function<bool(IntVec::value_type)> {
    return [&p1](IntVec::value_type pos) { return p1.second.first == pos; };
  };

  // create a list of all positions in the surfaceId to sheetPosition map that are outside the print range
  const auto                         prSheetsFirst = m_printSheetPositions.begin();
  const auto                         prSheetsLast = m_printSheetPositions.end();
  std::list<SheetPosMap::value_type> wastePositions;

  std::copy_if(m_surfaceIndexToAllSheetPositions.begin(), m_surfaceIndexToAllSheetPositions.end(), std::back_inserter(wastePositions),
               [=, &makePairCompare](SheetPosMap::value_type const& p) {
                 return std::find_if(prSheetsFirst, prSheetsLast, makePairCompare(p)) == prSheetsLast;
               }
              );

  // copy all non-redundant positions into a temporary map ...
  auto makePairCompare2 = [](SheetPosMap::value_type const& p1) -> std::function<bool(SheetPosMap::value_type const&)> {
    return [&p1](SheetPosMap::value_type const& p2) { return p1.second.first == p2.second.first; };
  };

  SheetPosMap tmpMap;
  std::copy_if(m_surfaceIndexToAllSheetPositions.begin(), m_surfaceIndexToAllSheetPositions.end(),
               std::inserter(tmpMap, tmpMap.end()),
               [&wastePositions, &makePairCompare2](SheetPosMap::value_type const& p) {
                 return std::find_if(wastePositions.begin(), wastePositions.end(), makePairCompare2(p)) == wastePositions.end();
               }
              );

  // ... and swap it with the complete map
  m_surfaceIndexToAllSheetPositions.swap(tmpMap);
}

void ImpoSurfaceSheetRelation::createSurfaceNumToIdMapping()
{
  if (m_surfaceIndexToPrintSheetPosition.size() != m_surfaceIndexToAllSheetPositions.size())
  {
    throw std::runtime_error("wrong number of surface Id's");
  }

  const auto surfNums = getMapKeys(m_surfaceIndexToPrintSheetPosition);
  const auto surfIds = getMapKeys(m_surfaceIndexToAllSheetPositions);
  for (size_t i{}; i < surfNums.size(); ++i)
  {
    m_surfaceIndexToId[surfNums[i]] = surfIds[i];
  }
}


} // namespace UPCL
