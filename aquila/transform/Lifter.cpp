#include "Lifter.h"
#include <algorithm>
#include <cmath>

#define PI 3.14159265358979323846

namespace Aquila
{
    Lifter::Lifter(std::size_t numCoeffs, int liftC):
        m_numCoeffs(numCoeffs), m_liftC(liftC)
    {
        for(std::size_t c = 0; c < m_numCoeffs; c++)
            m_lc.push_back(1 + (m_liftC/2)*std::sin(PI*c/m_liftC));
    }

    std::vector<double> Lifter::apply(const std::vector<double>& feat)
    {
        // check if feat and coeff vector have same dimensions
        std::vector<double> res(feat.size());
        for(std::size_t i = 0; i < feat.size(); i++)
            res[i] = feat[i] * m_lc[i];

        return res;
    }

}
