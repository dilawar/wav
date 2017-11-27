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
    if( argn > 1 )
        wav.parse( argv[1] );
    else
        wav.parse( "./kalia.wav" );

    std::streambuf * buf;
    std::ostream* fp;

    if( argn > 2 )
    {
        ofstream outF( argv[2], std::ofstream::out );
        buf = outF.rdbuf( );
    }
    else
        buf = std::cout.rdbuf( );

    
    std::ostream out( buf );

    auto channelData = wav.getData( );
    for( auto v : channelData )
    {
        out << v.first << ' ';
        for( auto vv : v.second )
            cout << vv << ' ';
        out << endl;
    }
    return 0;
}
