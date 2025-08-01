// Probability.cpp: implementation of the Probability class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "app/probability.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Probability::Probability()
{

	// Initialise the first value to 0
	// Just in case we don't insert a zero value

	chances.push_back ( 0 );

}

Probability::~Probability()
{

    chances.clear ();

}

void Probability::InputProbability ( int value, int percentagechance )
{

	if ( value >= chances.size () )
		chances.resize ( value + 1 );

	chances.insert( value, percentagechance);

}

void Probability::ChangeProbability ( int value, int percentagechance )
{

	if ( chances.ValidIndex (value) )
		chances.ChangeData( value, percentagechance);

	else
		printf ( "WARNING: Probability::ChangeProbability, invalid value passed\n" );

}

bool Probability::Validate ()
{

	int sum = 0;

	for ( int i = 0; i < chances.size (); ++i ) {

		if ( chances.ValidIndex ( i ) ) {

			int value = chances.at (i);

			if ( value >= 0 && value <= 100 )
				sum += value;

			else
				return false;

		}
		else {

			return false;

		}

	}
	
	return ( sum == 100 );
	
}

int Probability::GetValue ()
{

	int r = (int) ( 100 * ((float) rand () / (float) RAND_MAX) );

	int result = 0;
	int totalscore = 0;

	for ( int i = 0; i < chances.size (); ++i ) {
		if ( chances.ValidIndex ( i ) ) {
			if ( r >= totalscore && r < totalscore + chances.at (i) ) {
				result = i;
				break;
			}
			totalscore += chances.at (i);
		}
	}

	return result;

}
