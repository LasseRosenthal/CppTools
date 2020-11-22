//  ###################################################################
//
//  COPYRIGHT 2004 - 2018 Heidelberger Druckmaschinen AG
//       ALL RIGHTS RESERVED
//
//  SOURCE FILE: ImpoSurfaceSheetRelation.h
//
//  FILE DESCRIPTION:
//     Specification of the ImpoSurfaceSheetRelation class
//
//
//  ###################################################################

#ifndef IMPOSURFACESHEETRELATION_H
#define IMPOSURFACESHEETRELATION_H


// --------------------------------------------------------------------
//                             INCLUDES
// --------------------------------------------------------------------
// standard header
#include <algorithm>
#include <iterator>
#include <map>
#include <stdexcept>
#include <map>
#include <unordered_map>


namespace UPCL {


/**
 * @class ImpoSurfaceSheetRelation
 * @brief ImpoSurfaceSheetRelation provides a mapping between surface id's and sheets positions.
 */
class ImpoSurfaceSheetRelation {

  using IntVec = std::vector<int>;

public:

  /// Enumeration to tag sheet sides.
  enum class sheetSide : char {
    front, ///< tag for front
    back   ///< tag for the back side
  };

  /// Enumeration to specify simplex or duplex printing.
  enum class printMode : char { 
    simplex, ///< simplex printing
    duplex   ///< duplex printing
  };

  using sheetPosition = std::pair<int, sheetSide>;

  // ctor
  ImpoSurfaceSheetRelation       (int spreadType, IntVec printSheetPositions, IntVec allSheetPositions, IntVec tabSheetPositions);

  auto getSheetPosAndId          (int index) const -> std::pair<sheetPosition, int>;
  auto getSheetBySurfaceId       (int surfaceId) const -> sheetPosition;
  auto numPrintSurfaces          () const -> int;
  auto getPrintRangeIndices      () const -> std::vector<int>;
  auto isIndexValid              (int surfaceNumber) const -> bool;
  auto lowestPrintSurfaceIndex   () const -> int;



  // private types
  using SheetPosMap = std::map<int, sheetPosition>;
  using IntMap      = std::map<int, int>;
  using IntPair     = std::pair<int, int>;
  using IntPairVec  = std::vector<IntPair>;

  // members
  printMode   m_printMode;
  IntVec      m_printSheetPositions;
  IntVec      m_allSheetPositions;
  IntVec      m_tabSheetPositions;
  const int   m_numPrintSheets;
  int         m_numPrintSurfaces{-1};
  SheetPosMap m_surfaceIndexToPrintSheetPosition;
  SheetPosMap m_surfaceIndexToAllSheetPositions;
  IntMap      m_surfaceIndexToId;
  const int   m_biasAllSurfaceIndexing{0};
  const int   m_biasPrintSurfaceIndexing{0};

  // auxiliary methods
  auto spreadTypeToPrintMode                   (int spreadType) const->printMode;
  void initialize                              ();
  void createSurfaceNumbering                  (IntVec& sheetRange, SheetPosMap& surfaceToSheetMap, int startCtr = 0);
  void mapConsecutiveNumberingToSheetPositions (IntPairVec const&, SheetPosMap&, int startCtr);
  void reduceMappingToPrintRange               ();
  void createSurfaceNumToIdMapping             ();

  template <typename Map>
  auto getMapKeys (Map const&) const -> std::vector<typename Map::key_type>;

  template <typename InputIt>
  auto mapSheetpositionsToNumberOfSurfaces (InputIt printSheetBegin, const InputIt printSheetEnd,
                                            InputIt tabSheetFirst, const InputIt tabSheetLast) const->IntPairVec;
};


// --------------------------------------------------------------------
//                             INLINES
// --------------------------------------------------------------------
inline ImpoSurfaceSheetRelation::ImpoSurfaceSheetRelation(int spreadType, IntVec printSheetPositions,
                                                          IntVec allSheetPositions, IntVec tabSheetPositions)
  : m_printMode           {spreadTypeToPrintMode(spreadType)}
  , m_printSheetPositions {std::move(printSheetPositions)}
  , m_allSheetPositions   {std::move(allSheetPositions)}
  , m_tabSheetPositions   {std::move(tabSheetPositions)}
  , m_numPrintSheets      {static_cast<int>(m_printSheetPositions.size())}
{
  initialize();
}

/**
 * @brief returns the sheet position for a given zero based surface number.
 */
inline auto ImpoSurfaceSheetRelation::getSheetPosAndId(int index) const -> std::pair<sheetPosition, int>
{
  if (!isIndexValid(index))
  {
    throw std::runtime_error("invalid surface number.");
  }
  return std::make_pair(m_surfaceIndexToPrintSheetPosition.at(index), m_surfaceIndexToId.at(index));
}

/**
 * @brief returns the sheet position for a given surface id.
 */
inline auto ImpoSurfaceSheetRelation::getSheetBySurfaceId(int surfaceId) const -> sheetPosition
{
  const auto it = m_surfaceIndexToAllSheetPositions.find(surfaceId);
  if(it == m_surfaceIndexToAllSheetPositions.end())
  {
    throw std::runtime_error("invalid surface id.");
  }
  return it->second;
}

/**
 * @brief returns the number of surfaces to be printed.
 */
inline int ImpoSurfaceSheetRelation::numPrintSurfaces() const
{
  return m_numPrintSurfaces;
}

/**
 * @brief returns a vector containing the id's of all surfaces to be printed.
 */
inline std::vector<int> ImpoSurfaceSheetRelation::getPrintRangeIndices() const
{
  return getMapKeys(m_surfaceIndexToPrintSheetPosition);
}

/**
 * @brief checks if the surface number is valid.
 */
inline bool ImpoSurfaceSheetRelation::isIndexValid(int index) const
{
  return m_biasPrintSurfaceIndexing <= index && index < m_numPrintSurfaces + m_biasPrintSurfaceIndexing;
}

inline int ImpoSurfaceSheetRelation::lowestPrintSurfaceIndex() const
{
  return m_biasPrintSurfaceIndexing;
}

/**
 * @brief converts the int spreadType to the enum type printMode.
 */
inline auto ImpoSurfaceSheetRelation::spreadTypeToPrintMode(int spreadType) const -> printMode
{
  return spreadType == 1 ? printMode::simplex : printMode::duplex;
}

/**
 * @brief returns a vector containing all keys of the given map.
 */
template <typename Map>
auto ImpoSurfaceSheetRelation::getMapKeys(Map const& m) const -> std::vector<typename Map::key_type>
{
  std::vector<typename Map::key_type> v;
  v.reserve(m.size());
  std::transform(m.begin(), m.end(), std::back_inserter(v), [](typename Map::value_type const& val) { return val.first; });
  return v;
}

/**
 * @brief returns a vector containing pairs of sheetpositions and their corresponding number of surfaces
 *        (one for tabsheets and one(simplex) or two(duplex) for normal sheets).
 */
template <typename InputIt>
auto ImpoSurfaceSheetRelation::mapSheetpositionsToNumberOfSurfaces(InputIt printSheetBegin, const InputIt printSheetEnd,
                                                                   InputIt tabSheetFirst, const InputIt tabSheetLast) const -> IntPairVec
{
  IntPairVec allSheetsAndNumSurfaces;
  allSheetsAndNumSurfaces.reserve(std::distance(printSheetBegin, printSheetEnd));

  if(m_printMode != printMode::duplex)
  {
    std::transform(printSheetBegin, printSheetEnd, std::back_inserter(allSheetsAndNumSurfaces),
                   [](int pos) { return std::make_pair(pos, 1); });
  }
  else
  {
    auto duplexSheetGenerator = [](int pos) { return std::make_pair(pos, 2); };
    auto simplexSheetGenerator = [](int pos) { return std::make_pair(pos, 1); };
    auto inserter = std::back_inserter(allSheetsAndNumSurfaces);
    auto firstPosToSearch = printSheetBegin;
    auto nextTabSheet = firstPosToSearch;

    for(; tabSheetFirst != tabSheetLast; std::advance(tabSheetFirst, 1u))
    {
      nextTabSheet = std::find(firstPosToSearch, printSheetEnd, *tabSheetFirst);
      if(nextTabSheet != printSheetEnd)
      {
        // all sheets up to the next tab sheet are two sided
        std::transform(firstPosToSearch, nextTabSheet, inserter, duplexSheetGenerator);

        // include the tab sheet
        inserter = simplexSheetGenerator(*nextTabSheet);
        firstPosToSearch = std::next(nextTabSheet);
      }
    }

    // include the remaining duplex sheets if present
    std::transform(firstPosToSearch, printSheetEnd, inserter, duplexSheetGenerator);
  }

  return allSheetsAndNumSurfaces;
}


} // namespace UPCL


#endif // IMPOSURFACESHEETRELATION_H
