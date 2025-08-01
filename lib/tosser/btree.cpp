
// BTree implementation file
// Part of Tosser
// By Christopher Delay
// Modifications John Knottenbelt

#ifndef _included_tosser_btree
#define _included_tosser_btree

#include "tosser.h"

#include <assert.h>
#include <string.h>


template <class T>
BTree <T> :: BTree ()
{

    ltree = NULL;
    rtree = NULL;
    key = NULL;
    //data = NULL;
    data = 0;

}

template <class T>
BTree <T> :: BTree ( const char *newid, const T &newdata )
{
        
    ltree = NULL;
    rtree = NULL;
    key = new char [ strlen (newid) + 1 ];
    strcpy ( key, newid );
    data = newdata;
    
}


template <class T>
BTree<T> :: BTree ( const BTree<T> &copy )
{
    Copy( copy );
}

template <class T>
void BTree<T> :: Copy( const BTree<T> &copy )
{
    if (copy.ltree)	ltree = new BTree( *copy.ltree );
    else ltree = NULL;
    
    if (copy.rtree)	rtree = new BTree( *copy.rtree );
    else rtree = NULL;
    
    if (copy.key) {
		key = new char [ strlen (copy.key) + 1 ];
		strcpy ( key, copy.key );
    }
    else 
		key = NULL;
    
    data = copy.data;
}


template <class T>
BTree <T> :: ~BTree ()
{
    clear ();
}

template <class T>
void BTree <T> :: clear ()
{
    if (ltree) delete ltree;
    if (rtree) delete rtree;
    if (key)    delete [] key;
    
    ltree = rtree = NULL;
    key = NULL;
}

template <class T>
void BTree <T> :: insert ( const char *newid, const T &newdata )
{
    
    if ( !key ) {

        key = new char [ strlen ( newid ) + 1 ];
        strcpy ( key, newid );
        data = newdata;
	 	 
		return;

    }

    if ( strcmp ( newid, key ) <= 0 ) {

        if (ltree)
            ltree->insert ( newid, newdata );

        else
            ltree = new BTree ( newid, newdata );

    }
    else if ( strcmp ( newid, key ) > 0 ) {

        if (rtree)
            rtree->insert ( newid, newdata );

        else
            rtree = new BTree ( newid, newdata );

    }

}

template <class T>
void BTree <T> :: RemoveData ( const char *newid )
{
    
    /*
      Deletes an element from the list
      By replacing the element with it's own left node, then appending
      it's own right node onto the extreme right of itself.
      
      */
    
    assert (newid);
    
    if ( strcmp ( newid, key ) == 0 ) {
	
		//var tempright : pointer to node := data->right
		BTree <T> *tempright = Right ();       
			
			//data := data->left
		if ( Left () ) {
			
			key = new char [strlen (Left ()->key) + 1];
			strcpy ( key, Left ()->key );
			data = Left ()->data;                  // This bit requires a good copy constructor
			rtree = Left ()->Right ();
			ltree = Left ()->Left ();	
			
			AppendRight ( tempright );
			
		}
		else {
			
			//append_right ( data, tempright )
			
			if ( Right () ) {
			   
				key = new char [strlen (Right ()->key) + 1];
				strcpy ( key, Right ()->key );
				data = Right ()->data;                  // This bit requires a good copy constructor
				ltree = Right ()->Left ();	
				rtree = Right ()->Right ();
			
			}
			else {
			
				key = NULL;                              // Hopefully this is the root node
			
			}	    
			
		}
	    	
    }                                                   //elsif Name < data->name then
	else if ( strcmp ( newid, key ) < 0 ) {
		if ( Left () ) {
            if ( strcmp ( Left ()->key, newid ) == 0 && !Left ()->Left () && !Left ()->Right () )
                ltree = NULL;
            else
		        Left ()->RemoveData ( newid );
		}
    }
    else {                                              //elsif Name > data->name then
		if ( Right () ) {
            if ( strcmp ( Right ()->key, newid ) == 0 && !Right ()->Left () && !Right ()->Right () )
                rtree = NULL;
            else
		        Right ()->RemoveData ( newid );
		}
    }

}

template <class T>
void BTree <T> :: RemoveData ( const char *newid, const T &newdata  )
{
    
    /*
      Deletes an element from the list
      By replacing the element with it's own left node, then appending
      it's own right node onto the extreme right of itself.
      
      */
    
    assert (newid);
    
    if ( strcmp ( newid, key ) == 0 && data == newdata ) {
	
		//var tempright : pointer to node := data->right
		BTree <T> *tempright = Right ();       
			
			//data := data->left
		if ( Left () ) {
			
			key = new char [strlen (Left ()->key) + 1];
			strcpy ( key, Left ()->key );
			data = Left ()->data;                  // This bit requires a good copy constructor
			rtree = Left ()->Right ();
			ltree = Left ()->Left ();	
			
			AppendRight ( tempright );
			
		}
		else {
			
			//append_right ( data, tempright )
			
			if ( Right () ) {
			   
				key = new char [strlen (Right ()->key) + 1];
				strcpy ( key, Right ()->key );
				data = Right ()->data;                  // This bit requires a good copy constructor
				ltree = Right ()->Left ();	
				rtree = Right ()->Right ();
			
			}
			else {
			
				key = NULL;                              // Hopefully this is the root node
			
			}	    
			
		}
	    	
    }                                                   //elsif Name < data->name then
	else if ( strcmp ( newid, key ) <= 0 ) {
		if ( Left () ) {
            if ( strcmp ( Left ()->key, newid ) == 0 && data == newdata && !Left ()->Left () && !Left ()->Right () )
                ltree = NULL;
            else
		        Left ()->RemoveData ( newid, newdata );
		}
    }
    else {                                              //elsif Name > data->name then
		if ( Right () ) {
            if ( strcmp ( Right ()->key, newid ) == 0 && data == newdata && !Right ()->Left () && !Right ()->Right () )
                rtree = NULL;
            else
		        Right ()->RemoveData ( newid, newdata );
		}
    }

}

template <class T>
T BTree <T> :: at ( const char *searchid )
{

	BTree <T> *btree = find ( searchid );

	if ( btree ) return btree->data;
	else return NULL;

}


template <class T>
void BTree <T> :: AppendRight ( BTree <T> *tempright )
{
    
    if ( !Right () )
		rtree = tempright;
    
    else
		Right ()->AppendRight ( tempright );	
    
}

template <class T>
BTree<T> *BTree<T> :: find( const char *searchid )
{
        
    if (!key)
		return NULL;
    
    if ( strcmp ( searchid, key ) == 0 )
        return this;

    else if ( ltree && strcmp ( searchid, key ) < 0 )
        return ltree->find ( searchid );

    else if ( rtree && strcmp ( searchid, key ) > 0 )
        return rtree->find ( searchid );
    
    else return NULL;
    
}


template <class T>
int BTree <T> :: size () const
{

    unsigned int subsize = (key) ? 1 : 0;
    
    if (ltree) subsize += ltree->size ();
    if (rtree) subsize += rtree->size ();

    return subsize;

}

template <class T>
void BTree <T> :: Print ()
{
    
    if (ltree) ltree->Print ();
    if (key) cout << key << " : " << data << "\n";       
    if (rtree) rtree->Print ();
    
}
    
template <class T>
BTree <T> *BTree <T> :: Left () const
{

    return (BTree <T>*) ltree;

}

template <class T>
BTree <T> *BTree <T> :: Right () const
{

    return (BTree <T>*) rtree;

}

template <class T>
DArray <T> *BTree <T> :: MapDataToDArray ()
{
    
    DArray <T> *darray = new DArray <T>;
    RecursiveConvertToDArray ( darray, this );
    
    return darray;
    
}

template <class T>
DArray <char *> *BTree <T> :: MapKeysToDArray ()
{
    
    DArray <char *> *darray = new DArray <char *>;
    RecursiveConvertIndexToDArray ( darray, this );
    
    return darray;
    
}

template <class T>
void BTree <T> :: RecursiveConvertToDArray ( DArray <T> *darray, BTree <T> *btree )
{
    
    assert (darray);
    
    if ( !btree ) return;            // Base case
    
    if ( btree->key ) darray->push_back ( btree->data );
    
    RecursiveConvertToDArray ( darray, btree->Left  () );
    RecursiveConvertToDArray ( darray, btree->Right () );
    
}

template <class T>
void BTree <T> :: RecursiveConvertIndexToDArray ( DArray <char *> *darray, BTree <T> *btree )
{
    
    assert (darray);
    
    if ( !btree ) return;            // Base case
    
    if ( btree->key ) darray->push_back( btree->key );
    
    RecursiveConvertIndexToDArray ( darray, btree->Left  () );
    RecursiveConvertIndexToDArray ( darray, btree->Right () );
    
}

#endif
