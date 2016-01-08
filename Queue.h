// Queue.h : Interface for a first-in, first-out (FIFO) container class.

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <list>
using std::list;

template <typename Item>
class Queue : private list<Item>
{
	private:
		// What's the maximum number of Items to hold?
		const unsigned int Max;

	public:
		// Constructor.
		//
		// In:		max	The maximum number of Items to hold,
		//			or 0 if (essentially) unlimited.
		Queue(unsigned int max = 0) : Max(max)
		{
		}

		// Enqueue (add) an Item to the Queue.
		//
		// In:		i	The Item to add!
		//
		// Returns:		True if the Item was added!
		bool Enqueue(const Item &i);

		// Dequeue (remove) an Item from the Queue.
		//
		// In:	i		Where to put the front Node's data.
		//
		// Out:	i		The changed Item.
		//
		// Returns		True if successful.
		bool Dequeue(Item &i);

		// How many things are in the Queue?
		//
		// Returns		The number of things in the Queue.
		unsigned int Size(void) const { return static_cast<unsigned int>(size()); }

		// Is the Queue empty?
		//
		// Returns		True if the Queue is empty.
		bool isEmpty(void) const { return empty(); }

		// Is the Queue "full"?
		//
		// Returns		True if the Queue is empty.
		bool isFull(void) const { return Max && size() == Max; }

		// "Flush".
		void Destroy()
		{
			clear();
		}
};

// Enqueue (add) an Item to the Queue.
//
// In:		i	The Item to add!
//
// Returns:		True if the Item was added!
template <typename Item>
bool Queue<Item>::Enqueue(const Item &i)
{
	if (Max && Max <= size())
		// There's no "room".
		return false;

	push_back(i);
	return true;
}

// Dequeue (remove) an Item from the Queue.
//
// In:	i		Where to put the front Node's data.
//
// Out:	i		The changed Item.
//
// Returns		True if successful.
template <typename Item>
bool Queue<Item>::Dequeue(Item &i)
{
	if (empty())
		// Nothin' to dequeue!
		return false;

	i = front();
	pop_front();
	return true;
}

#endif