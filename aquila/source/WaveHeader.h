//

#ifndef AQUILA_WAVEHEADER_H
#define AQUILA_WAVEHEADER_H

#include <cstdint>

namespace Aquila
{
/**
 * Which channel to use when reading stereo recordings.
 */
    enum StereoChannel
    {
        LEFT, RIGHT
    };

/**
 * .wav file header structure.
 */
    struct WaveHeader
    {
        char RIFF[4];
        std::uint32_t DataLength;
        char WAVE[4];
        char fmt_[4];
        std::uint32_t SubBlockLength;
        std::uint16_t formatTag;
        std::uint16_t Channels;
        std::uint32_t SampFreq;
        std::uint32_t BytesPerSec;
        std::uint16_t BytesPerSamp;
        std::uint16_t BitsPerSamp;
        char data[4];
        std::uint32_t WaveSize;
    };
}
#endif //AQUILA_WAVEHEADER_H
