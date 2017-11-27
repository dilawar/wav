/***
 *       Filename:  test_wav.cpp
 *
 *    Description:  Test the header file.
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

#include "WAVReader.h"
#include <fstream>
#include <cassert>

using namespace std;

int main(int argn, const char *argv[])
{
    WAVE wav;
    bool toFile = false;
    if( argn > 1 )
        wav.parse( argv[1] );
    else
        wav.parse( "./kalia.wav" );

    std::streambuf * buf;
    std::ostream* fp;

    ofstream outF;
    if( argn > 2 )
    {
        outF.open( argv[2], std::ofstream::out );
        toFile = true;
    }
    
    auto channelData = wav.getData( );
    for( auto v : channelData )
    {
        if( toFile )
        {
            outF << v.first << ' ';
            for( auto vv : v.second )
                outF << vv << ' ';
            outF << '\n';
        }
        else
        {
            cout << v.first << ' ';
            for( auto vv : v.second )
                cout << vv << ' ';
            cout << '\n';
        }
    }
    return 0;
}
