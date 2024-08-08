#ifndef GENERATE_TERRAIN
#define GENERATE_TERRAIN

#include <cstdint>

/**
 * @brief Generate a terrain
 * @param IDs Block IDs (output, must be freed after use)
 * @param IDIndexes Start index for each (x, z) in IDs (output, must be freed after use)
**/
void generateTerrain(int*& IDs, uint32_t*& IDIndexes);

#endif