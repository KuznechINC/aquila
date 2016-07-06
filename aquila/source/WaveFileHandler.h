/**
 * @file WaveFileHandler.h
 *
 * A utility class to handle loading and saving of .wav files.
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

#ifndef WAVEFILEHANDLER_H
#define WAVEFILEHANDLER_H

#include "../global.h"
#include "SignalSource.h"
#include <cstddef>
#include <string>
#include <fstream>

namespace Aquila
{
    /**
     * Forward declaration to avoid including WaveFile.h header.
     */
    struct WaveHeader;

    /**
     * A utility class to handle loading and saving of .wav files.
     */
    class AQUILA_EXPORT WaveFileHandler
    {
    public:
        WaveFileHandler(const std::string& filename);

        void readHeader(WaveHeader& header);
        void readHeaderAndChannels(WaveHeader& header,
            ChannelType& leftChannel,
            ChannelType& rightChannel);
        void readPart(const WaveHeader& header,
            ChannelType& leftChannel,
            ChannelType& rightChannel,
            size_t partSize);

        void save(const SignalSource& source);

        void decodeData(const WaveHeader& header, short* data, size_t channelSize,
                        ChannelType& leftChannel, ChannelType& rightChannel);

        static void decode16bit(ChannelType& channel,
            short* data, std::size_t channelSize);
        static void decode16bitStereo(ChannelType& leftChannel,
            ChannelType& rightChannel, short* data, std::size_t channelSize);

        static void decode8bit(ChannelType& channel,
            short* data, std::size_t channelSize);
        static void decode8bitStereo(ChannelType& leftChannel,
            ChannelType& rightChannel, short* data, std::size_t channelSize);

        static void encode16bit(const SignalSource& source, short* data, std::size_t dataSize);
        static void encode8bit(const SignalSource& source, short* data, std::size_t dataSize);

    private:
        void createHeader(const SignalSource& source, WaveHeader& header);
        static void splitBytes(short twoBytes, unsigned char& lb, unsigned char& hb);

        /**
         * Destination or source file.
         */
        const std::string m_filename;
        std::fstream m_fs_handle;
    };
}

#endif // WAVEFILEHANDLER_H
