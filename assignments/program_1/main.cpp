/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*	Author: Shady Boukhary													+
*	Email: sb199898.sb@gmail.com									+
*	Files: main.cpp													+
*	3013- Advanced Structures and Algorithms						+
*	Due January 25th, 2018											+
*	Dr. Terry Griffin												+
*	Purpose: Simple program to revise implementation of priority	+
*	queue. The program initializes a list with random numbers		+
*	inserted in ascending order. Finally, it prints that list to	+
*	the console.													+
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#include <iostream>
#include <ctime>

using namespace std;

//Create a container for our list data
struct node {
	int data;
	node* next;
};

/**
* Class: intLinkedList
* Purpose:
*     Implements a singly linked list that holds integers.
* Methods:
*     void  frontSert(int x)
*	  void	orderedSert(int x)
*     node* find(int key)
*     node* remove(int key)
*     void  print()
*/
class intLinkedList {
private:

	node* Head;
public:

	intLinkedList() {
		Head = NULL;
	}


	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*							frontSert()								+
	*	@params: integer												+
	*	@return: none													+
	*	@usage: frontSert(x);											+
	*	@Purpose: inserts integer to the front of the list				+
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	void frontSert(int x) {
		//empty list case
		if (!Head) {
			Head = new node;
			Head->data = x;
			Head->next = NULL;
		}
		else {//not empty list
			node* T = new node;
			T->data = x;
			T->next = Head;
			Head = T;
		}
	}

	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*							orderedSert()							+
	*	@params: integer												+
	*	@return: none													+
	*	@usage: mylist.orderedSert(rand() % 100);						+
	*	@Purpose: inserts integer to an ordered linked list of			+
	*	ascending order													+
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	void orderedSert(int x) {

		// if list is empty do a front insert
		if (!Head) {
			frontSert(x);
		}
		// if list is not empty
		else {

			// find node of same priority if available
			node* result = Find(x);
			node* toInsert = new node;
			node * temp = Head;
			toInsert->data = x;

			// if node of same priority exists and is not the head node
			if (result && result != Head) {

				// traverse to index of that node - 1
				while (temp->next != result)
					temp = temp->next;
				// insert node behind the found node
				toInsert->next = temp->next;
				temp->next = toInsert;
			}

			// if no node of equal priority was found
			else {
				// if new node data is less than head node do a front insert
				if (toInsert->data <= temp->data) {
					toInsert->next = Head;
					Head = toInsert;
				}

				// if new node data is greater than head node data 
				else {
					// traverse the list until a greater node data is found 
					while (temp->next && toInsert->data > temp->next->data)
						temp = temp->next;
					// insert new node behind found node
					toInsert->next = temp->next;
					temp->next = toInsert;
				}
			}
		}
	}


	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*								Find()								+
	*	@params: integer												+
	*	@return: node*													+
	*	@usage: node* result = Find(x); 								+
	*	@Purpose: searches linked list for a node containing a 			+
	*	specific integer												+
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	node* Find(int key) {
		node* temp = Head;
		while (temp) {
			if (temp->data == key) {
				return temp;
			}
			temp = temp->next;
		}
		return NULL;
	}


	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*								Remove()							+
	*	@params: integer												+
	*	@return: none													+
	*	@usage: mylist.Remove(58); 										+
	*	@Purpose: removes selected node from the list		 			+
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	void Remove(int key) {
		node* result = Find(key);
		if (result) {
			node* temp = Head;
			while (temp->next != result) {
				temp = temp->next;
			}
			temp->next = result->next;
			delete result;
		}
	}

	/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*								print()								+
	*	@params: none													+
	*	@return: none													+
	*	@usage: mylist.print(); 										+
	*	@Purpose: dumps the entire list to console			 			+
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	void print() {
		node* p = Head;
		while (p) {
			cout << p->data << endl;
			p = p->next;
		}
	}

};

int main() {
	// seed random number generator
	srand(8734587);

	// ummmm
	cout << "Hello World!\n";

	// declare instance of intLinkedList turning
	// a class definition into an "object"
	intLinkedList mylist;

	//Load 10 random ints into our list
	for (int i = 0; i < 100; i++) {
		mylist.orderedSert(rand() % 100);
	}

	//print the list
	mylist.print();

	//find some value
	cout << mylist.Find(97) << endl;

	//remove some value 
	mylist.Remove(58);
	cout << endl << endl;

	//print the list again
	mylist.print();

	system("pause");

	return 0;
}