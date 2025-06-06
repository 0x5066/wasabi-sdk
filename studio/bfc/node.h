/*

  Nullsoft WASABI Source File License

  Copyright 1999-2001 Nullsoft, Inc.

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.


  Brennan Underwood
  brennan@nullsoft.com

*/

#ifndef _NODE_H
#define _NODE_H

#include "../bfc/ptrlist.h"

//
//		node.h
//
//	Some node classes.
//



// ==========================================================================
//
//		class Node
//
//	A generic "node" object from which one may subclass and create a set of
//	disparate objects that may be assembled into a hierarchical tree.
//
class Node {
private:
	PtrList<Node>	  							childlist;
	Node *												parent;

protected:

#ifdef  _DEBUG
	static int										count;
#endif//_DEBUG

public:

	Node(Node * myParent = NULL) : childlist(), parent(myParent) { 
#ifdef  _DEBUG
		count++; 
#endif//_DEBUG
	}

	Node(const Node & a) : childlist(a.childlist), parent(a.parent) { 
#ifdef  _DEBUG
		count++; 
#endif//_DEBUG
	}

	virtual ~Node() { 
#ifdef  _DEBUG
		count--; 
#endif//_DEBUG
	};

	int nodeCount() {
#ifdef  _DEBUG
		return count;
#else //_DEBUG
		return -1;
#endif//_DEBUG
	}

	//
	//	Const Data Accessor Methods
	int getNumChildren() const { 
		return childlist.getNumItems(); 
	}

	Node * enumChild(int index) const { 
		return childlist[index];
	}

	Node * getParent() const {
		return parent;
	}

	//
	//	Nonconst Data Manipulator Methods
	Node * addChild(Node * child) {
		child->setParent(this);
		return childlist.addItem(child);
	}

	Node * setParent(Node * myParent) {
		return parent = myParent;
	}

	PtrList< Node >	& ChildList() {
		return childlist;
	}
};

// ==========================================================================
//
//		class NodeC< TPayload >
//
//	If you would rather use Node as a container class than as a base class, 
//	this object will allow you to contain a payload instance and be accessed
//	as if it were the payload instance using a reference operator and an
//	implicit cast operator.
//
template < class TPayload >
class NodeC : public Node {
protected:
	TPayload								payload;
public:
	NodeC( const TPayload & myPayload, NodeC * myParent = NULL ) : Node( myParent ), payload( myPayload ) {}
	NodeC( const NodeC & a ) : Node( a ), payload( a.payload ) {}

	//
	// In addition to being able to call all of the Node methods, you can also
	// simply treat the node objects themselves as if they were the payload 
	// instantiation through these simple methods:

	// Explicit reference operator - for lvalues and force rvalues.
	TPayload & operator () ( void ) { return payload; }
	// Implicit cast operator - for rvalues that can accept a TPayload &
	operator TPayload &  ( void ) { return payload; }
};

#endif//_NODE_H
