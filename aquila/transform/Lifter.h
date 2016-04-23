#ifndef LIFTER_H
#define LIFTER_H

#include "../global.h"
#include <vector>
#include <cstddef>

namespace Aquila
{

    class AQUILA_EXPORT Lifter
    {
    public:
        Lifter(std::size_t numCoeffs, int liftC);
        std::vector<double> apply(const std::vector<double>& feat);
    private:
        std::size_t m_numCoeffs;
        int m_liftC;
        std::vector<double> m_lc;
    };
}
#endif
