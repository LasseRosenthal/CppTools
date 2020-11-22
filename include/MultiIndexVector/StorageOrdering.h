/* -------------------------------------------------------------------------- 
 * Copyright (c) 2020 Heidelberger Druckmaschinen AG 
 * Reproduction or disclosure of this file or its contents without the prior 
 * written consent of Heidelberger Druckmaschinen AG is prohibited. 
 * -------------------------------------------------------------------------- */ 
 
/** 
 * @file    StorageOrdering.h 
 * @brief   definition of the storageOrdering enumeration
 * 
 * @author  Lasse Rosenthal 
 * @date    23.09.2020 
 */ 
 
#ifndef STORAGEORDERING_H_24761125092440912634134151939516664615929367 
#define STORAGEORDERING_H_24761125092440912634134151939516664615929367


namespace multiindex {


/// storageOrdering is an enumeration that specifies different methods of storing multidimensional arrays in linear storage.
enum class storageOrdering : char {
   rowMajor,    ///< row-major ordering or lexicographic ordering
   columnMajor  ///< column-major ordering or colexicographic ordering 
};


}   // namespace multiindex


// *************************************************************************** // 
// ******************************* END OF FILE ******************************* // 
// *************************************************************************** // 
 
#endif // STORAGEORDERING_H_24761125092440912634134151939516664615929367 
