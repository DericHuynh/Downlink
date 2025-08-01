
#include "tosser.h"

#include "app/serialise.h"

#include "world/computer/security.h"
#include "world/computer/securitysystem.h"

#include "mmgr.h"


Security::Security ()
{
}

Security::~Security ()
{

    DeleteDArrayData ( (DArray <UplinkObject *> *) &systems );

}


void Security::AddSystem ( SecuritySystem *newsystem, int index )
{

	if ( index == -1 )
		systems.push_back (newsystem);

	else
		systems.insert(index, newsystem);

}

void Security::AddSystem ( int TYPE, int level, int index )
{

	SecuritySystem *ss = new SecuritySystem ();
	ss->SetTYPE ( TYPE );
	ss->SetLevel ( level );
	ss->Enable ();

	AddSystem ( ss, index );

}

void Security::RemoveAllSystems ()
{

    systems.clear ();

}

SecuritySystem *Security::GetSystem (int index)
{

	if ( systems.ValidIndex (index) )
		return systems.at (index);

	else
		return NULL;

}

int Security::NumSystems ()
{

	return systems.size ();

}

int Security::NumRunningSystems ()
{

	int numsystems = 0;

	for ( int i = 0; i < systems.size (); ++i )
		if ( systems.ValidIndex (i) )
			if ( systems.at (i)->enabled &&
				!systems.at (i)->bypassed )
				++numsystems;

	return numsystems;

}

bool Security::IsRunning ( int TYPE )
{

	for ( int i = 0; i < systems.size (); ++i ) 
		if ( systems.ValidIndex (i) )
			if ( systems.at (i) )
				if ( systems.at (i)->TYPE == TYPE )
					if ( systems.at (i)->enabled && 
						!systems.at (i)->bypassed )
						return true;

	return false;

}
	
bool Security::IsRunning_Proxy ()
{

	return IsRunning ( SECURITY_TYPE_PROXY );

}

bool Security::IsRunning_Firewall ()
{
	
	return IsRunning ( SECURITY_TYPE_FIREWALL );

}

bool Security::IsRunning_Encryption ()
{

	return IsRunning ( SECURITY_TYPE_ENCRYPTION );

}

bool Security::IsRunning_Monitor ()
{

	return IsRunning ( SECURITY_TYPE_MONITOR );

}

bool Security::IsAnythingEnabled ()
{

	for ( int i = 0; i < systems.size (); ++i ) 
		if ( systems.ValidIndex (i) )
			if ( systems.at (i) )
				if ( systems.at (i)->enabled == true )
					return true;

	return false;

}

bool Security::IsAnythingDisabled ()
{

	for ( int i = 0; i < systems.size (); ++i ) 
		if ( systems.ValidIndex (i) )
			if ( systems.at (i) )
				if ( systems.at (i)->enabled == false )
					return true;

	return false;

}

bool Security::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDArray ( (DArray <UplinkObject *> *) &systems, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Security::Save  ( FILE *file )
{

	SaveID ( file );

	SaveDArray ( (DArray <UplinkObject *> *) &systems, file );

	SaveID_END ( file );

}

void Security::Print ()
{

	printf ( "Security : \n" );
	PrintDArray ( (DArray <UplinkObject *> *) &systems );

}

void Security::Update ()
{
	
}

char *Security::GetID ()
{

	return "SECUR";

}

