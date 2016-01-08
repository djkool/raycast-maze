// Stack.cpp - Implementation of a First-In, Last-Out (FILO) Stack container.

#include "Stack.h"

// Destructor
Stack::~Stack(void)
{
	// Free all the Nodes, if any...
	Node *temp = this->front;
	while (this->front)
	{
		// Delete each node, one at a time.
		this->front = this->front->next;
		delete temp;
		temp = this->front;
	}
}

// Push (add) something to the Stack
//
// In:		i	The Item to add to the Stack.
void Stack::push(const Item &i)
{
	// Create our new node and fill it out.
	Node *temp = new Node;
	temp->data = i;
	temp->prev = 0;

	// Link up the new node.
	if (this->front)
		this->front->prev = temp;
	temp->next = this->front;
	this->front = temp;
}

// Pop (remove) something FROM the Stack
//
// In:		i	Where to put the front Node's data.
//
// Out:		i	The filled-in Item.
//
// Returns		True if there was at least one Item
//				in the Stack.
bool Stack::pop(Item &i)
{
	// Is there anything in the Stack?
	if (!this->front)
		return false;

	// Fill in the Item the user passed...
	i = this->front->data;

	// Kill that front node.
	Node *temp = this->front;
	this->front = this->front->next;
	delete temp;

	// Make sure that wasn't the last Node...
	if (this->front)
		this->front->prev = 0;

	// Success!
	return true;
}