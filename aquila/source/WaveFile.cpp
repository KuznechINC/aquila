/**
 * @file WaveFile.cpp
 *
 * WAVE file handling as a signal source.
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
 * @since 0.0.7
 */

#include "WaveFile.h"

namespace Aquila
{
    /**
     * Creates a wave file object and immediately loads data from file.
     *
     * @param filename full path to .wav file
     * @param channel LEFT or RIGHT (the default setting is LEFT)
     */
    WaveFile::WaveFile(const std::string& filename, StereoChannel channel):
        SignalSource(), m_filename(filename), m_handler(filename)
    {
        load(m_filename, channel);
    }

    WaveFile::WaveFile(const std::string& filename, size_t part_size, StereoChannel channel):
        SignalSource(), m_filename(filename), m_partSize(part_size), m_handler(filename)
    {
        //load(m_filename, channel);
        m_handler.readHeader(m_header);
    }

    /**
     * Deletes the WaveFile object.
     */
    WaveFile::~WaveFile()
    {
    }

    /**
     * Reads the header and channel data from given .wav file.
     *
     * Data are read into a temporary buffer and then converted to
     * channel sample vectors. If source is a mono recording, samples
     * are written to left channel.
     *
     * To improve performance, no format checking is performed.
     *
     * @param filename full path to .wav file
     * @param channel which audio channel to read (for formats other than mono)
     */
    void WaveFile::load(const std::string& filename, StereoChannel channel)
    {
        m_filename = filename;
        m_data.clear();
        ChannelType dummy;
        //WaveFileHandler handler(m_filename);
        if (LEFT == channel)
        {
            m_handler.readHeaderAndChannels(m_header, m_data, dummy);
        }
        else
        {
            m_handler.readHeaderAndChannels(m_header, dummy, m_data);
        }
        m_sampleFrequency = m_header.SampFreq;
    }

    std::vector<std::vector<double>> WaveFile::load_next()
    {
        std::vector<std::vector<double>> channel_data;
        ChannelType l_chan;
        ChannelType r_chan;
        m_handler.readPart(m_header, l_chan, r_chan, m_partSize);
        if (m_header.Channels == 1)
        {
            channel_data.push_back(l_chan);
        }
        else if (m_header.Channels == 2)
        {
            channel_data.push_back(l_chan);
            channel_data.push_back(r_chan);
        }
        m_sampleFrequency = m_header.SampFreq;

        return channel_data;
    }

    /**
     * Saves the given signal source as a .wav file.
     *
     * @param source source of the data to save
     * @param filename destination file
     */
    void WaveFile::save(const SignalSource& source, const std::string& filename)
    {
        WaveFileHandler handler(filename);
        handler.save(source);
    }

    /**
     * Returns the audio recording length
     *
     * @return recording length in milliseconds
     */
    unsigned int WaveFile::getAudioLength() const
    {
        return static_cast<unsigned int>(m_header.WaveSize /
                static_cast<double>(m_header.BytesPerSec) * 1000);
    }

    unsigned int WaveFile::getNumParts() const
    {
        return static_cast<unsigned int>(m_header.WaveSize/
                static_cast<double>(m_partSize));
    }
}
