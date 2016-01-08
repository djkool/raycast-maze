// Stack.h - Interface for a First-In, Last-Out (FILO) Stack container.

#ifndef _STACK_H_
#define _STACK_H_

#include <exception>

template <typename Item>
class Stack
{
private:
	// Each link in the chain...
	struct Node
	{
		// What are we holdin?
		Item data;

		// Who's the Node after this one?
		Node *next;

		// Who's the Node before this one?
		Node *prev;

		Node(const Item &data)
			: data(data), next(0), prev(0) { }

		// Return:	Our next pointer.
		const Node * getNext() const
		{
			return this->next;
		}
	};

	// Who is the first Node in the Stack?
	Node *front;

	// No copies allowed!
	Stack(const Stack &s);
	Stack &operator=(const Stack &s);

public:
	// Constructor
	Stack(void)
	{
		// Nuttin' in the Stack yet!
		this->front = 0;
	}
	// Destructor
	~Stack(void);

	// Push (add) something to the Stack
	//
	// In:		i	The Item to add to the Stack.
	void push(const Item &i);

	// Pop (remove) something FROM the Stack
	//
	// In:		i	Where to put the front Node's data.
	//
	// Out:		i	The filled-in Item.
	//
	// Returns		True if there was at least one Item
	//				in the Stack.
	bool pop(Item &i);
};

// Destructor
template <typename Item>
Stack<Item>::~Stack(void)
{
	// Free all the Nodes, if any...
	Node *temp = this->front;
	while (this->front)
	{
		// Delete each node, one at a time.
		this->front = const_cast<Node *>(this->front->getNext());
		delete temp;
		temp = this->front;
	}
}

// Push (add) something to the Stack
//
// In:		i	The Item to add to the Stack.
template <typename Item>
void Stack<Item>::push(const Item &i)
{
	try
	{
		// Create our new node and fill it out.
		Node *temp = new Node(i);
		if (!temp)
			return;

		// Link up the new node.
		if (this->front)
			this->front->prev = temp;
		temp->next = this->front;
		this->front = temp;
	}
	catch (std::bad_alloc &ba)
	{
		con << ba.what() << '\n';
		throw;
	}
}

// Pop (remove) something FROM the Stack
//
// In:		i	Where to put the front Node's data.
//
// Out:		i	The filled-in Item.
//
// Returns		True if there was at least one Item
//				in the Stack.
template <typename Item>
bool Stack<Item>::pop(Item &i)
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

#endif