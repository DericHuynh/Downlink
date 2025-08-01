
//===============================================================//
//                        T O S S E R                            //
//                                                               //
//                   By Christopher Delay                        //
//                           V1.14                               //
//===============================================================//

#ifndef _included_tosser_h
#define _included_tosser_h

#include <iostream>
using namespace std;

#include "mmgr.h"

/*
#ifdef _DEBUG
#include "slasher.h"
#endif
*/


//=================================================================
// Linked list object
// Source :: llist.cc
// Use : A dynamicly sized list of data
// NOT sorted in any way - new data is simply added on at the end
// Indexes of data are not constant
// Sequential access is fast, random access is slow

template <class T>
class LListItem
{

protected:

public:

	T data;
	LListItem *next;
	LListItem *previous;

	LListItem ();
	~LListItem ();

};

template <class T>
class LList
{

protected:

	LListItem <T> *first;                 // Pointer to first node
	LListItem <T> *last;                  // Pointer to last node

	LListItem <T> *previous;              // Used to get quick access
	int previousindex;                    // for sequential reads (common)

	int numitems;

public:

	LList ();
	~LList ();

	void push_back   ( const T &newdata ); // Adds in data at the end	
	void PutDataAtStart ( const T &newdata );	
	void insert(int index, const T &newdata);

    T at          ( int index );			// slow unless sequential
	void erase    ( int index );			// slow 
    int  FindData      ( const T &data );		// -1 means 'not found'
  
    int size ();			 // Returns the total size of the array
    bool ValidIndex ( int index );

    void clear ();				 // Resets the array to empty    
    
    T operator [] (int index);

};

//=================================================================
// Dynamic array object
// source :: darray.cc
// Use : A dynamically sized list of data
// Which can be indexed into - an entry's index never changes

template <class T>
class DArray
{

protected:

    int arraysize;
    T * dynarray;
    char * shadow;				 //0=not used, 1=used
    
public:

    DArray ();
    DArray ( const DArray<T>& da );
    ~DArray ();

    void resize ( int newsize );

    int  push_back    ( const T &newdata );			 // Returns index used
    void insert    ( int index, const T &newdata);
    T    at    ( int index ) const;
    void ChangeData(int index, const T &newdata);
    void erase ( int index );
    int  FindData   ( const T &data ) const;		 // -1 means 'not found'
    
    int NumUsed () const;		 // Returns the number of used entries
    int size () const;			 // Returns the total size of the array
    
    bool ValidIndex ( int index ) const;		    // Returns true if the index contains used data
    
    void clear ();				 // Resets the array to empty    
    
    T& operator [] (int index);

    typedef int ( * Comparator ) ( const T *data1, const T *data2 );

    void Sort ( Comparator comp ); 

};

//=================================================================
// Binary tree object
// source :: btree.cc
// Use : A sorted dynamic data structure
// Every data item has a string id which is used for ordering
// Allows very fast data lookups

template <class T>
class BTree
{

protected :

    BTree *ltree;
    BTree *rtree;
    
    void RecursiveConvertToDArray ( DArray <T> *darray, BTree <T> *btree );
    void RecursiveConvertIndexToDArray ( DArray <char *> *darray, BTree <T> *btree );
    
    void AppendRight ( BTree <T> *tempright );                            // Used by Remove
    
public :

    char *key;
    T data;

    BTree ();
    BTree ( const char *newkey, const T &newdata );
    BTree ( const BTree<T> &copy );

    ~BTree ();
    void Copy( const BTree<T> &copy );

    void insert ( const char *newkey, const T &newdata );
    void RemoveData ( const char *newkey );                     // Requires a solid copy constructor in T class
    void RemoveData ( const char *newkey, const T &newdata );   // Requires a solid copy constructor in T class
    T at ( const char *searchkey );

    BTree *find( const char *searchkey );
    
    void clear ();
    
    int size () const;							 // Returns the size in elements
    
    void Print ();                              // Prints this tree to stdout
    
    BTree *Left () const;
    BTree *Right () const;
    
    DArray <T> *MapDataToDArray ();
    DArray <char *> *MapKeysToDArray ();
    
};




//  ===================================================================

#include "llist.cpp"
#include "darray.cpp"
#include "btree.cpp"

#include "nommgr.h"

#endif
