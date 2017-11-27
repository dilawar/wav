/***
 *       Filename:  WAVEReader.hpp
 *
 *    Description:  WAVE file parser.
 *
 *        Version:  0.0.1
 *        Created:  2017-11-25
 *       Revision:  none
 *
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *
 *        License:  GNU GPL2
 */

#ifndef WAVE_H
#define WAVE_H

#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <fstream>

using namespace std;


class WAVE
{
    public:
        WAVE(){ };
        ~WAVE(){ };

        WAVE( const string& filename )
        {
            parse( filename );
        }

        int parse( const string& filepath )
        {
            return parse( filepath.c_str( ) );
        }

        void printHeader( )
        {

        }

        int parse( const char* filepath )
        {
            ifstream fh;
            fh.open( filepath, fh.binary | fh.in );
            
            /******************************************************************
             * Get the chunkID ; it is 4 byte.
             *
             ******************************************************************
             */
            array<uint8_t, 4> chunk;
            readBytes<uint8_t, 4>( fh, chunk );

            // Assert that it is RIFF.
            array<uint8_t, 4> riff = { 'R', 'I', 'F', 'F' };
            if( chunk != riff )
            {
                cerr << "Error: Not a valid WAVE file." << endl;
                return -1;
            }

            /******************************************************************
             * Get the chunkID ; it is 4 byte.
             *
             ******************************************************************
             */
            fh.read( reinterpret_cast<char*>(&chunk_size_), sizeof chunk_size_ );

            /******************************************************************
             * Next for characters contains WAVE
             *
             ******************************************************************
             */
            readBytes<uint8_t, 4>( fh, chunk );
            // Assert that it is RIFF.
            array<uint8_t, 4> wave = { 'W', 'A', 'V', 'E' };
            if( chunk != wave )
            {
                cerr << "Error: Not a valid WAVE file." << endl;
                fh.close( );
                return -1;
            }

            /******************************************************************
             * Subchunk ID.
             *
             ******************************************************************
             */
            readBytes<uint8_t, 4>( fh, chunk );
            // Assert that it is RIFF.
            array<uint8_t, 4> fmt = { 'f', 'm', 't', ' ' };
            if( chunk != fmt )
            {
                cerr << "Error: Not a valid WAVE file." << endl;
                fh.close( );
                return -1;
            }

            /*******************************************************************
             * Rest of the parameters.
             *******************************************************************
             */
            fh.read( reinterpret_cast<char*>(&subchunk1_size_)
                    , sizeof subchunk1_size_);
            if( subchunk1_size_ != 16 )
            {
                std::cerr << "Fatal: PCM != 16. Not supported." << std::endl;
                fh.close();
                return -1;
            }
            fh.read( reinterpret_cast<char*>(&audio_format_)
                    , sizeof audio_format_ );

            if( audio_format_ != 1 )
            {
                cerr << "Warning: This file compressed. Not supported" << endl;
                fh.close( );
                return -1;
            }

            fh.read( reinterpret_cast<char*>(&num_channels_)
                    , sizeof num_channels_ );
            fh.read( reinterpret_cast<char*>(&sample_rate_)
                    , sizeof sample_rate_ );
            fh.read( reinterpret_cast<char*>(&byte_rate_)
                    , sizeof byte_rate_ );
            fh.read( reinterpret_cast<char*>(&block_align_)
                    , sizeof block_align_ );
            fh.read( reinterpret_cast<char*>(&bits_per_sample_)
                    , sizeof bits_per_sample_ );
                
            /* data section */
            readBytes<uint8_t, 4>( fh, chunk );
            // Assert that it is RIFF.
            array<uint8_t, 4> dataH = { 'd', 'a', 't', 'a' };
            if( chunk != dataH )
            {
                cerr << "Error: Not a valid WAVE file." << endl;
                fh.close( );
                return -1;
            }

            // This is the size of data.
            fh.read( reinterpret_cast<char*>(&subchunk2_size_)
                    , sizeof subchunk2_size_ );

            // Now we can read data (little endian format).
            // If bits_per_sample_ == 8 then unsigned int are stored.
            // If bits_per_sample_ == 16, then 2's complement signed int is
            // stored.
            if( 8 == bits_per_sample_ )
            {
                uint8_t data;
                for (size_t i = 0; i < subchunk2_size_; i++) 
                {
                    fh.read( reinterpret_cast<char*>(&data), 1 );
                    values_.push_back( data );
                }
            }
            else if( 16 == bits_per_sample_ )
            {

                uint16_t data;
                for (size_t i = 0; i < subchunk2_size_ / 2; i++) 
                {
                    fh.read( reinterpret_cast<char*>(&data), 2 );
                    values_.push_back(0x8000&data ? (int)(0x7FFF&data)-0x8000 : data);
                }
            }
            else
            {
                cerr << "BIT PER SAMPLE " << sample_rate_ << " not supported."
                    << endl;
                return -2;
            }
            fh.close( );

            // Number of bystes to read.
            unsigned int stride = bits_per_sample_ / 8;

            // Now reconstuct data into int format;
            double time;
            for (size_t i = 0; i < values_.size( ) / num_channels_; i++) 
            {
                vector<int> chdata( num_channels_ );
                time = 1.0 * i / sample_rate_;

                for (size_t ii = 0; ii < num_channels_; ii++) 
                    chdata[ii] = values_[ num_channels_ * i + ii ];

                channel_data_.push_back( make_pair( time, chdata ) );
            }

            return 0;
        }

        template<typename T, unsigned int N>
        void readBytes( ifstream& fh, array<T, N>& arr )
        {
            uint8_t ch;
            for (size_t i = 0; i < N; i++) 
            {
                fh.read( reinterpret_cast<char*>(&ch), 1 );
                arr[i] = ch;
            }
        }

        vector< pair<double, vector<int>> > getData( )
        {
            return channel_data_;
        }

    private:
        /* data */
        vector<pair<double, vector<int>>> channel_data_;
        vector<int> values_;

        uint32_t chunk_size_;
        uint32_t subchunk1_size_;
        uint16_t audio_format_;
        uint16_t num_channels_;

        uint32_t sample_rate_;
        uint32_t byte_rate_;

        uint16_t block_align_;
        uint16_t bits_per_sample_;

        uint32_t subchunk2_size_;


};
   
#endif /* end of include guard: WAVE_H */
