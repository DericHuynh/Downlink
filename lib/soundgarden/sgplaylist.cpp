
#include <stdio.h>

#include "sgplaylist.h"

#include "mmgr.h"



SgPlaylist::SgPlaylist ()
{

    sprintf ( name, "New playlist" );
            
}

SgPlaylist::~SgPlaylist ()
{

	for ( int i = 0; i < songs.size (); ++i ) 
		if ( songs.at (i) )
			if ( strlen(songs.at (i)) != 0 )
				if ( strcmp(songs.at (i), "") != 0 )
					delete [] songs.at (i);

}


void SgPlaylist::SetName ( char *newname )
{

    strcpy ( name, newname );

}

void SgPlaylist::AddSong ( char *name )
{

    char *namecopy = new char [strlen(name)+1];
    sprintf ( namecopy, name );
    songs.push_back(namecopy);

}

int SgPlaylist::NumSongs ()
{
    
    return songs.size ();

}

char *SgPlaylist::GetRandomSong ()
{

  	int songindex = (int)( ( (float) rand () / (float) RAND_MAX ) * NumSongs () );
    return songs.at(songindex);

}

char *SgPlaylist::GetRandomSong ( char *oldsong )
{

    if ( !oldsong ) {

        return GetRandomSong ();

    }
    else if ( NumSongs () == 1 ) {

        return songs.at (0);

    }
    else {

        char *candidate = GetRandomSong ();
        while ( strcmp ( candidate, oldsong ) == 0 )
            candidate = GetRandomSong ();

        return candidate;

    }

}
