# wav

Header only uncompressed WAV file parser.

## Snippet 

    WAVE wav( "./kalia.wav" );
    
    // channelData is vector< pair<double, vector<int> > 
    auto channelData = wav.getData( );
    for( auto v : channelData )
    {
        cout << v.first << ' ';
        for( auto vv : v.second )
            cout << vv << ' ';
        cout << endl;
     }
     
`channelData` is a vector of `pair`. First value is time and second value is a vector of values
in channels. 

__NOTE__ Only tested with simple uncompressed wav file with 1 channel. 
