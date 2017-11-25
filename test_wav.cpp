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

int main(int argc, const char *argv[])
{
    WAVE wav( "./kalia.wav" );

    auto channelData = wav.getData( );
    for( auto v : channelData )
    {
        cout << v.first << ' ';
        for( auto vv : v.second )
            cout << vv << ' ';
        cout << endl;
    }



    
    return 0;
}
