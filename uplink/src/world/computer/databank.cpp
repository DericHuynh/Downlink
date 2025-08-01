// DataBank.cpp: implementation of the DataBank class.
//
//////////////////////////////////////////////////////////////////////

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/databank.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataBank::DataBank()
{
	
	formatted = false;

}

DataBank::~DataBank()
{

    DeleteDArrayData ( (DArray <UplinkObject *> *) &data );
    
}

void DataBank::SetSize ( int newsize )
{

	memory.resize ( newsize );

}

int DataBank::GetSize ()
{

	return memory.size ();

}

int DataBank::NumDataFiles ()
{

	return data.NumUsed ();

}

int DataBank::GetDataSize ()
{

	return	data.size ();

}

bool DataBank::PutData ( Data *newdata )
{

	UplinkAssert (newdata);

	int index = FindValidPlacement ( newdata );

	if ( index != -1 ) PutData ( newdata, index );

	return ( index != -1 );

}

void DataBank::InsertData ( Data *newdata )
{

    UplinkAssert (newdata);

    if ( data.size () == 0 )
        PutData ( newdata );

    else {

        int insertindex = NumberGenerator::RandomNumber ( data.size () );

        // Move the old data item from this location to the end

        if ( data.ValidIndex (insertindex) ) {
            data.resize ( data.size () + 1 );
            Data *olddata = data.at (insertindex);
            UplinkAssert (olddata);
            int newindex = data.size () - 1;
            data.insert(newindex, olddata);

            for ( int i = 0; i < memory.size (); ++i )
                if ( memory.ValidIndex (i) )
                    if ( memory.at (i) == insertindex )
                        memory.insert( i, newindex);
        }
        
        // Insert the new data item

        data.insert( insertindex, newdata);

        int index = FindValidPlacement (newdata);

        if ( index == -1 ) {
            memory.resize ( memory.size () + newdata->size );
            index = FindValidPlacement (newdata);
        }
    
        UplinkAssert (index != -1);

        for ( int i = 0; i < newdata->size; ++i )
		    memory.insert( index + i, insertindex);

    }

}

void DataBank::PutData ( Data *newdata, int memoryindex )
{

	int pos = data.push_back ( newdata );

	UplinkAssert ( newdata );

	for ( int i = 0; i < newdata->size; ++i )
		memory.insert( memoryindex + i, pos);

}

void DataBank::RemoveData ( int memoryindex )
{

	if ( memory.ValidIndex (memoryindex) ) {

		// Delete the file

		int dataindex = GetDataIndex (memoryindex);
		
		RemoveDataFile ( dataindex );

	}

}

void DataBank::RemoveDataFile ( int dataindex )
{

	if ( data.ValidIndex (dataindex) ) {

		// Delete the file

        Data *thefile = data.at (dataindex);               // TODO is it safe to delete the data here?
        if ( thefile ) delete thefile;

		data.erase (dataindex);

		// Delete any indexes to the file
	
		for ( int i = 0; i < memory.size (); ++i )
			if ( memory.ValidIndex (i) )
				if ( memory.at (i) == dataindex )
					memory.erase ( i );

		// If that was the last file, then this databank has been formatted

		if ( NumDataFiles () == 0 )
			formatted = true;

	}

}

Data *DataBank::GetData ( int memoryindex )
{

	if ( !memory.ValidIndex (memoryindex) )
		return NULL;

	else if ( !data.ValidIndex ( memory [memoryindex] ) )
		return NULL;

	else
		return data [ memory [memoryindex] ];

}

Data *DataBank::GetData ( char *title )
{

	for ( int i = 0; i < data.size (); ++i )
		if ( data.ValidIndex (i) )
			if ( strcmp ( data.at (i)->title, title ) == 0 )
				return data.at (i);

	return NULL;

}

bool DataBank::ContainsData ( char *title, float version )
{

	for ( int i = 0; i < data.size (); ++i )
		if ( data.ValidIndex (i) )
			if ( strcmp ( data.at (i)->title, title ) == 0 ) {
				float versionData = data.at (i)->version;
				if ( versionData < 0.0f || versionData >= version )
					return true;
			}

	return false;

}

Data *DataBank::GetDataFile ( int dataindex )
{

	if ( !data.ValidIndex (dataindex) )
		return NULL;

	else
		return data.at (dataindex);

}

int DataBank::GetDataIndex ( int memoryindex )
{

	if ( !memory.ValidIndex (memoryindex) )
		return -1;

	else if ( !data.ValidIndex ( memory [memoryindex] ) )
		return -1;

	else
		return memory [memoryindex];
	

}

int DataBank::GetMemoryIndex ( int dataindex )
{

	for ( int i = 0; i < memory.size (); ++i ) {
		if ( memory.ValidIndex (i) ) {
	
			if ( memory.at (i) == dataindex )
				return i;

		}
	}

	return -1;

}

int DataBank::IsValidPlacement ( Data *newdata, int memoryindex )
{

	UplinkAssert (newdata);
	
	if ( memoryindex < 0 ) return 2;

	for ( int i = memoryindex; i < memoryindex + newdata->size; ++i ) {
	
		if ( i >= memory.size () || i < 0 ) 
			return 2;									// Not valid

		else if ( memory.ValidIndex (i) )
			return 1;									// Will overwrite

	}

	return 0;											// No conflicts

}

int DataBank::FindValidPlacement ( Data *newdata )
{

	UplinkAssert (newdata);

	for ( int i = 0; i < memory.size (); ++i ) {

		if ( IsValidPlacement ( newdata, i ) == 0 ) {

			return i;
		
		}

	}

	return -1;

}

void DataBank::Format ()
{

    int oldmemsize = memory.size ();

	DeleteDArrayData ( (DArray <UplinkObject *> *) &data );
	
	data.clear ();
	memory.clear ();
    memory.resize ( oldmemsize );

	formatted = true;

}

void DataBank::RandomizeDataPlacement ()
{

	DArray <Data *> tempdata;

	for ( int i = 0; i < data.size (); i++ )
		if ( data.ValidIndex ( i ) )
			tempdata.push_back ( data.at ( i ) );

	for ( int i = 0; i < tempdata.size (); i++ ) {
		int index1 = NumberGenerator::RandomNumber ( tempdata.size () );
		int index2 = NumberGenerator::RandomNumber ( tempdata.size () );

		if ( index1 != index2 && tempdata.ValidIndex ( index1 ) && tempdata.ValidIndex ( index2 ) ) {
			Data *temp = tempdata.at ( index1 );
			tempdata.insert( index1, tempdata.at ( index2 ));
			tempdata.insert( index2, temp);
		}
	}

	data.clear ();

	int oldmemsize = memory.size ();
	memory.clear ();
    memory.resize ( oldmemsize );

	int index = 0;
	for ( int i = 0; i < tempdata.size (); i++ )
		if ( tempdata.ValidIndex ( i ) ) {
			PutData ( tempdata.at ( i ), index );
			index += tempdata.at ( i )->size;
		}

}

bool DataBank::Load ( FILE *file )
{
	
	LoadID ( file );

	if ( !LoadDArray ( (DArray <UplinkObject *> *) &data, file ) ) return false;
	if ( !LoadDArray ( &memory, file ) ) return false;

	if ( !FileReadData ( &formatted, sizeof(formatted), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void DataBank::Save ( FILE *file )
{

	SaveID ( file );

	SaveDArray ( (DArray <UplinkObject *> *) &data, file );
	SaveDArray ( &memory, file );

	fwrite ( &formatted, sizeof(formatted), 1, file );

	SaveID_END ( file );

}

void DataBank::Print ()
{

	printf ( "DataBank\n" );
	PrintDArray ( (DArray <UplinkObject *> *) &data );
	PrintDArray ( &memory );
	printf ( "Formatted: %d\n", formatted );

}
	
char *DataBank::GetID ()
{

	return "DATABANK";

}

int DataBank::GetOBJECTID ()
{

	return OID_DATABANK;

}

void DataBank::Update ()
{

//	UpdateDArray ( (DArray <UplinkObject *> *) &data );

}


// Data class =================================================================


Data::Data ()
{

	UplinkStrncpy ( title, "Empty", sizeof ( title ) );
	TYPE = DATATYPE_NONE;
	size = 0;
	encrypted = 0;
	compressed = 0;
	version = 1.0;
	softwareTYPE = SOFTWARETYPE_NONE;

}

Data::Data ( Data *copyme )
{

	UplinkAssert (copyme);

	SetTitle ( copyme->title );
	TYPE = copyme->TYPE;
	size = copyme->size;
	encrypted = copyme->encrypted;
	compressed = copyme->compressed;
	version = copyme->version;
	softwareTYPE = copyme->softwareTYPE;

}

Data::~Data ()
{
}

void Data::SetTitle ( char *newtitle )
{
	
	UplinkAssert ( strlen (newtitle) < SIZE_DATA_TITLE );
	UplinkStrncpy ( title, newtitle, sizeof ( title ) );

}

void Data::SetDetails ( int newTYPE, int newsize, int newencrypted, int newcompressed, 
					    float newversion, int newsoftwareTYPE )
{

	TYPE = newTYPE;
	size = newsize;
	encrypted = newencrypted;
	compressed = newcompressed;
	version = newversion;
	softwareTYPE = newsoftwareTYPE;

}

bool Data::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringStatic ( title, SIZE_DATA_TITLE, file ) ) return false;

	if ( !FileReadData ( &TYPE, sizeof(TYPE), 1, file ) ) return false;
	if ( !FileReadData ( &size, sizeof(size), 1, file ) ) return false;
	if ( !FileReadData ( &encrypted, sizeof(encrypted), 1, file ) ) return false;
	if ( !FileReadData ( &compressed, sizeof(compressed), 1, file ) ) return false;
	if ( !FileReadData ( &version, sizeof(version), 1, file ) ) return false;
	if ( !FileReadData ( &softwareTYPE, sizeof(softwareTYPE), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Data::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( title, SIZE_DATA_TITLE, file );

	fwrite ( &TYPE, sizeof(TYPE), 1, file );
	fwrite ( &size, sizeof(size), 1, file );
	fwrite ( &encrypted, sizeof(encrypted), 1, file );
	fwrite ( &compressed, sizeof(compressed), 1, file );
	fwrite ( &version, sizeof(version), 1, file );
	fwrite ( &softwareTYPE, sizeof(softwareTYPE), 1, file );

	SaveID_END ( file );

}

void Data::Print ()
{
	
	printf ( "Data: %s\n", title );
	printf ( "type = %d, size = %d, encrypted = %d, compressed = %d\nversion = %f, softwareTYPE = %d\n", 
				TYPE, size, encrypted, compressed, version, softwareTYPE );

}

void Data::Update ()
{
}

char *Data::GetID ()
{
	
	return "DATA";

}

int Data::GetOBJECTID ()
{

	return OID_DATA;

}


