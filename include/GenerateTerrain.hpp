#ifndef GENERATE_TERRAIN
#define GENERATE_TERRAIN

#include <cstdint>
#include <vector>

/**
 * @brief Generate a terrain
 * @param IDs Block IDs
 * @param IDIndexes Start index for each (x, z) in IDs
**/
void generateTerrain(std::vector<int>& IDs, uint32_t* IDIndexes);

#endif