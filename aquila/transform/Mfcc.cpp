/**
 * @file Mfcc.cpp
 *
 * Calculation of MFCC signal features.
 *
 * This file is part of the Aquila DSP library.
 * Aquila is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package Aquila
 * @version 3.0.0-dev
 * @author Zbigniew Siciarz
 * @date 2007-2014
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 3.0.0
 */

#include "Mfcc.h"
#include "Dct.h"
#include "Lifter.h"
#include "../source/SignalSource.h"
#include "../filter/MelFilterBank.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/core/core.hpp>

namespace Aquila
{
    /**
     * Calculates a set of MFCC features from a given source.
     *
     * @param source input signal
     * @param numFeatures how many features to calculate
     * @return vector of MFCC features of length numFeatures
     */
    std::vector<double> Mfcc::calculate(const SignalSource &source,
                                        std::size_t numFeatures,
                                        size_t cur_frame,
                                        size_t debug_frame)
    {
        //auto spectrum = m_fft->fft(source.toArray());

        cv::Mat inp(cv::Mat::zeros(m_inputSize, 1, CV_64F));
        const int n = std::min(source.length(), m_inputSize);
        for(int i = 0; i < n; i++)
            inp.at<double>(i, 0) = source.toArray()[i];

        cv::Mat res(std::ceil(m_inputSize/2), 2, CV_64FC1);
        cv::dft(inp, res, cv::DFT_COMPLEX_OUTPUT);
        SpectrumType spectrum;
        for(int i = 0; i < res.rows; i++)
            spectrum.push_back(std::complex<double>(res.at<double>(i,0), res.at<double>(i,1)));

        std::vector<double> pspec = periodogram(spectrum);
        double eng = std::accumulate(pspec.begin(), pspec.end(), 0.0);

        FrequencyType lowF = 0;
        FrequencyType highF = source.getSampleFrequency()/2;
        FrequencyType melLowF = Aquila::MelFilter::linearToMel(lowF);
        FrequencyType melHighF = Aquila::MelFilter::linearToMel(highF);
        FrequencyType filterWidth = 2*(melHighF - melLowF) / (double)(m_numFilters+1);

        Aquila::MelFilterBank bank(source.getSampleFrequency(), m_inputSize, filterWidth, m_numFilters);
        auto filterOutput = bank.applyAll(pspec);

        std::transform(filterOutput.begin(), filterOutput.end(), filterOutput.begin(), 
                      [this](double fv)->double
                      {
                          return fv > 0? std::log(fv) : std::log(m_eps); 
                      }
                     );

        Aquila::Dct dct;
        std::vector<double> dcted = dct.dct(filterOutput, numFeatures);

        Aquila::Lifter lifter(numFeatures, m_lifterCoeff);
        std::vector<double> lifted = lifter.apply(dcted);
        lifted[0] = eng > 0? std::log(eng) : std::log(m_eps);

        return lifted;
    }

    std::vector<double> Mfcc::periodogram(const SpectrumType& spectrum)
    {
        std::size_t numCoeffs = static_cast<std::size_t>(std::ceil(spectrum.size()/2));
        std::vector<double> pspec(numCoeffs);
        for(std::size_t i = 0; i < numCoeffs; i++)
            pspec[i] = 1/double(m_inputSize) * std::pow(std::abs(spectrum[i]), 2);

        return pspec;
    }
}
