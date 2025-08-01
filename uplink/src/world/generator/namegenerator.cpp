
#include <stdio.h>

#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/company/company.h"
#include "world/computer/computer.h"
#include "world/generator/worldgenerator.h"
#include "world/generator/namegenerator.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"


LList <char *> NameGenerator::fornames;
LList <char *> NameGenerator::surnames;
LList <char *> NameGenerator::agentaliases;
LList <char *> NameGenerator::companynamesA;
LList <char *> NameGenerator::companynamesB;


static char tempname [MAX_COMPUTERNAME];                 // This is used to return string values


void NameGenerator::Initialise ()
{
}

void NameGenerator::Shutdown ()
{

    DeleteLListData ( &fornames );
    DeleteLListData ( &surnames );
    DeleteLListData ( &agentaliases );
    DeleteLListData ( &companynamesA );
    DeleteLListData ( &companynamesB );

    fornames.clear ();
    surnames.clear ();
    agentaliases.clear ();
    companynamesA.clear ();
    companynamesB.clear ();

}

void NameGenerator::LoadNames ()
{

	//
	// Load fornames (only required once)
	//

	char tempName [ 128 ];

	if ( fornames.size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/fornames.txt", "rt" );
		UplinkAssert ( file );

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			char *name = new char [ strlen ( tempName ) + 1 ];
			UplinkSafeStrcpy ( name, tempName );
			fornames.push_back ( name );

		}
		
        char *deleteme = fornames.at ( fornames.size () - 1 );
        delete [] deleteme;
        fornames.erase ( fornames.size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/fornames.txt", file );

	}

	//
	// Load surnames (required when all surnames have been used)
	//

	if ( surnames.size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/surnames.txt", "rt" );
		UplinkAssert ( file );

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			char *name = new char [ strlen ( tempName ) + 1 ];
			UplinkSafeStrcpy ( name, tempName );
			surnames.push_back ( name );

		}

        char *deleteme = surnames.at ( surnames.size () - 1 );
        delete [] deleteme;
		surnames.erase ( surnames.size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/surnames.txt", file );

	}

	//
	// Load Agent aliases
	//

	if ( agentaliases.size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/agentaliases.txt", "rt" );
		UplinkAssert ( file );

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			char *name = new char [ strlen ( tempName ) + 1 ];
			UplinkSafeStrcpy ( name, tempName );
			agentaliases.push_back ( name );

		}

        char *deleteme = agentaliases.at ( agentaliases.size () - 1 );
        delete [] deleteme;
		agentaliases.erase ( agentaliases.size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/agentaliases.txt", file );

	}

	//
	// Load company names (partA)
	// These may run out and will then need to be reloaded
	//

	if ( companynamesA.size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/companya.txt", "rt" );
		UplinkAssert ( file );

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			char *name = new char [ strlen ( tempName ) + 1 ];
			UplinkSafeStrcpy ( name, tempName );
			companynamesA.push_back ( name );

		}

        char *deleteme = companynamesA.at ( companynamesA.size () - 1 );
        delete [] deleteme;
		companynamesA.erase ( companynamesA.size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/companya.txt", file );

	}

	//
	// Load company names (partB)
	//

	if ( companynamesB.size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/companyb.txt", "rt" );
		UplinkAssert ( file );

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			char *name = new char [ strlen ( tempName ) + 1 ];
			UplinkSafeStrcpy ( name, tempName );
			companynamesB.push_back ( name );

		}

        char *deleteme = companynamesB.at ( companynamesB.size () - 1 );
        delete [] deleteme;
		companynamesB.erase ( companynamesB.size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/companyb.txt", file );

	}

}

char *NameGenerator::GenerateCompanyName ()
{

	LoadNames ();

	int size1 = companynamesA.size ();
	int size2 = companynamesB.size ();


	// Choose part 1 (primary name part1)
	int index1 = NumberGenerator::RandomNumber ( size1 );

	// Choose part 2 (primary name part2)
	int index2 = NumberGenerator::RandomNumber ( size2 );

	// Build the name 

    char *companynameA = companynamesA.at (index1);
    char *companynameB = companynamesB.at (index2);
	char name [SIZE_COMPANY_NAME];
	UplinkSnprintf ( name, sizeof ( name ), "%s %s", companynameA, companynameB );

	delete [] companynameA;
    companynamesA.erase (index1);

    UplinkStrncpy ( tempname, name, sizeof ( tempname ) );
	return tempname;

}

char *NameGenerator::GeneratePersonName ()
{

	LoadNames ();								// Won't do anything if names exist

	int size_f = fornames.size ();
	int size_s = surnames.size ();
	
	int index_f = NumberGenerator::RandomNumber ( size_f );
	int index_s = NumberGenerator::RandomNumber ( size_s );

	// Build the name;

    char *forname = fornames.at (index_f);
    char *surname = surnames.at (index_s);
	char name [SIZE_PERSON_NAME];
	UplinkSnprintf ( name, sizeof ( name ), "%s %s", forname, surname );

    delete [] surname;
	surnames.erase (index_s);

    UplinkStrncpy ( tempname, name, sizeof ( tempname ) );
	return tempname;

}

char *NameGenerator::GenerateAgentAlias ()
{

	LoadNames ();

	int index = NumberGenerator::RandomNumber ( agentaliases.size () );

	char *result = agentaliases.at (index);
    UplinkStrncpy ( tempname, result, sizeof ( tempname ) );

    delete [] result;
	agentaliases.erase (index);

	return tempname;

}

char *NameGenerator::GenerateBankName ()
{

	return GenerateCompanyName ();

}

char *NameGenerator::GeneratePassword ()
{

	int numpasswords = game->GetWorld ()->passwords.size ();

	int index = NumberGenerator::RandomNumber ( numpasswords );

	while (!(game->GetWorld ()->passwords.ValidIndex (index)) ) 
		index = NumberGenerator::RandomNumber ( numpasswords );
			
	return game->GetWorld ()->passwords.at (index);

}

char *NameGenerator::GenerateEasyPassword ()
{

	switch ( NumberGenerator::RandomNumber ( 5 ) ) {

		case 0		:		return "god";
		case 1		:		return "admin";
		case 2		:		return "password";
		case 3		:		return "love";
		case 4		:		return "secure";

		default:	
			UplinkAbort ( "You've fucked up" );

	}

	return "Shut_the_compiler_up";

}

char *NameGenerator::GenerateComplexPassword ()
{

	// Get a normal password
	// change up to half the letters

	char password [9];
	UplinkStrncpy ( password, GeneratePassword (), sizeof ( password ) );

	int numchanges = NumberGenerator::RandomNumber ( 5 );

	for ( int i = 0; i < numchanges; ++i ) {

		int letternumber = NumberGenerator::RandomNumber ( 8 );
		char newcharacter = NumberGenerator::RandomNumber ( 26 ) + 'a';

		*(password + letternumber) = newcharacter;

	}

    UplinkStrncpy ( tempname, password, sizeof ( tempname ) );
	return tempname;

}

char *NameGenerator::GenerateDataName ( char *companyname, int DATATYPE )
{

	UplinkAssert (companyname);

	char dataname [SIZE_DATA_TITLE];

	switch ( DATATYPE ) {

		case DATATYPE_NONE:
		{

			UplinkSnprintf ( dataname, sizeof ( dataname ), "%c%c%c-file-%d", companyname [0], companyname [1], companyname [2],
																				NumberGenerator::RandomNumber ( 99999 ) );
			break;

		}
		case DATATYPE_DATA:
		{
			
			UplinkSnprintf ( dataname, sizeof ( dataname ), "%c%c%c-data-%d", companyname [0], companyname [1], companyname [2],
																				NumberGenerator::RandomNumber ( 99999 ) );
			break;

		}
		case DATATYPE_PROGRAM:
		{

			UplinkSnprintf ( dataname, sizeof ( dataname ), "%c%c%c-prog-%d", companyname [0], companyname [1], companyname [2],
																				NumberGenerator::RandomNumber ( 99999 ) );
			break;

		}

	}

    UplinkStrncpy ( tempname, dataname, sizeof ( tempname ) );
	return tempname;

}

char *NameGenerator::GeneratePublicAccessServerName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s Public Access Server", companyname );
    return tempname;
}

char *NameGenerator::GenerateAccessTerminalName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s Access Terminal", companyname );
    return tempname;
}

char *NameGenerator::GenerateInternalServicesServerName ( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s Internal Services Machine", companyname );
    return tempname;  
}

char *NameGenerator::GenerateCentralMainframeName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s Central Mainframe", companyname );
    return tempname;
}

char *NameGenerator::GenerateInternationalBankName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s International Bank", companyname );
    return tempname;
}

char *NameGenerator::GenerateFileServerName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s File Server", companyname );
    return tempname;
}

char *NameGenerator::GenerateLANName( char *companyname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s Local Area Network", companyname );
    return tempname;
}

char *NameGenerator::GeneratePersonalComputerName( char *personname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s's Personal Computer", personname );
    return tempname;
}

char *NameGenerator::GenerateVoicePhoneSystemName( char *personname )
{
    UplinkSnprintf( tempname, sizeof ( tempname ), "%s's Voice Phone System", personname );
    return tempname;
}

