//*****************************************************************
//  LinkedList.cpp
//  HashTable
//
//  Created by Karlina Beringer on June 16, 2014.
//
//  This header file contains the Linked List class declaration.
//  Hash Table array elements consist of Linked List objects.
//*****************************************************************

#include "LinkedList.h"

// Constructs the empty linked list object.
// Creates the head node and sets length to zero.
LinkedList::LinkedList()
{
    head = new Item;
    head -> next = NULL;
    length2 = 0;
}

//Inserts an item at the end of the list.
void LinkedList::insertItem( Item * newItem )
{
    if (!head -> next)
    {
        head -> next = newItem;
        length2++;
        return;
    }
    Item * p = head;
    //Item * q = head;
    while (p)
    {
        p = p -> next;
    }
    p = newItem;
    p -> next = NULL;
    length2++;
}

// Removes an item from the list by item key.
// Returns true if the operation is successful.
bool LinkedList::removeItem( string itemKey )
{
    if (!head -> next) return false;
    Item * p = head;
    Item * q = head;
    while (q)
    {
        if (q -> key == itemKey)
        {
            p -> next = q -> next;
            delete q;
            length2--;
            return true;
        }
        p = q;
        q = p -> next;
    }
    return false;
}

// Searches for an item by its key.
// Returns a reference to first match.
// Returns a NULL pointer if no match is found.
Item * LinkedList::getItem( string itemKey )
{
    Item * p = head;
    //Item * q = head;
    while (p)
    {
        //p = q;
        if ((p != head) && (p -> key == itemKey))
            return p;
        p = p -> next;
    }
    return NULL;
}

// Displays list contents to the console window.
void LinkedList::printList()
{
	Item * p = head;

    /*if (length2 == 0)*/
	if (NULL == p)
    {
        cout << "\n{ }\n";
        return;
    }
    
    //Item * q = head;
    cout << "\n{ ";
    while (p)
    {
        //p = q;
        if (p != head)
        {
            cout << p -> key;
			if (p->next){
				cout << ", ";
			}
			else{
				cout << " ";
			}
        }
        p = p -> next;
    }
    cout << "}\n";
}

// Returns the length of the list.
int LinkedList::getLength()
{
    return length2;
}

// De-allocates list memory when the program terminates.
LinkedList::~LinkedList()
{
    Item * p = head;
    Item * q = head;
    while (q)
    {
        p = q;
        q = p -> next;
        if (q) delete p;
    }
}

//*****************************************************************
// End of File
//*****************************************************************


















