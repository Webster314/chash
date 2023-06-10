#ifndef _P128_H_
#define _P128_H_
#include <iostream>

void out128(std::ostream& out, __uint128_t val, int neg);
std::ostream& operator<<(std::ostream& out, __uint128_t val);
std::ostream& operator<<(std::ostream& out, __int128_t val);

#endif