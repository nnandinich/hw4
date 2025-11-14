#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key,Value>* n1);
    void rotateLeft(AVLNode<Key,Value>* n1);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* prev=NULL;
    AVLNode<Key, Value>* curr=static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::root_);
    while(curr!=NULL){
      if((new_item.first)>(curr->getKey())){
        prev=curr;
        curr=curr->getRight();
      }
      //if insert is less than go left and progress forward
      else if((new_item.first)<(curr->getKey())){
        prev=curr;
        curr=curr->getLeft();
      }
      //key alread in tree, so change value
      else{
        curr->setValue(new_item.second);
        return;
      }
    }
  //create new node
  AVLNode<Key, Value>* insertNode = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
  //connect new node to parent
  insertNode->setParent(prev);
  //if tree empty, make new node root
  if(prev==NULL){
    this->root_=insertNode;
    return;
  }
  //new node is rightChild
  else if(new_item.first > prev->getKey()){
    prev->setRight(insertNode);
    prev->updateBalance(1);
  }
  //new node is LeftChild
  else{
    prev->setLeft(insertNode);
    prev->updateBalance(-1);
  }
  if(prev->getBalance()==0){
    return;
  }
  insertFix(prev, insertNode);
}


//fixes left heavy
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* n1){  
  if(n1==NULL){
    return;
  }
  //save original parents
  AVLNode<Key, Value>* ogParent = n1->getParent();
  //left child is new parent
  AVLNode<Key, Value>* leftChild = n1->getLeft();
  if(leftChild==NULL){
    return;
  }
  //this is n1's new left subtree
  AVLNode<Key, Value>* rightSubTree = leftChild->getRight();
  //move leftchild right subtree to n1 left subtree
  n1->setLeft(rightSubTree);
  if(rightSubTree!=NULL){
    rightSubTree->setParent(n1);
  }
  //update parent
  leftChild->setRight(n1);
  n1->setParent(leftChild);

  ////make n1 parent leftChild
  leftChild->setParent(ogParent);
  //n1 was root
  if(ogParent==NULL){
    this->root_=leftChild;
  }
  //n1 is left child
  else if(ogParent->getLeft()==n1){
    ogParent->setLeft(leftChild);
  }
  //n1 is right child
  else{
    ogParent->setRight(leftChild);
  }

}

//fixes right heavy
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* n1){
  if(n1==NULL){
    return;
  }
  //save original parents
  AVLNode<Key, Value>* ogParent = n1->getParent();
  //right child is new parent
  AVLNode<Key, Value>* rightChild = n1->getRight();
  if(rightChild==NULL){
    return;
  }
  //this is n1's new right subtree
  AVLNode<Key, Value>* leftSubTree = rightChild->getLeft();
  //move rightchild left subtree to n1 right subtree
  n1->setRight(leftSubTree);
  //update parent
  if(leftSubTree!=NULL){
    leftSubTree->setParent(n1);
  }
  //make n1 parent rightChild
  rightChild->setLeft(n1);
  n1->setParent(rightChild);

  rightChild->setParent(ogParent);
  //n1 was root
  if(ogParent==NULL){
    this->root_=rightChild;
  }
  //n1 is right child
  else if(ogParent->getRight()==n1){
    ogParent->setRight(rightChild);
  }
  //n1 is left child
  else{
    ogParent->setLeft(rightChild);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
  //g is parent of p
  //p is parent of n
  if(p==NULL){
    return;
  }
  AVLNode<Key, Value>* g = p->getParent();
  if(g==NULL){
    return;
  }
  //left child
  if(g->getLeft()==p){
    g->updateBalance(-1);
    if(g->getBalance()==0){
      return;
    }
    if(g->getBalance()==-1){
      insertFix(g,p);
      return;
    }
    if(g->getBalance()==-2){
      //zig-zig (left-left)
      if(p->getLeft()==n){
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      //zig-zag (left-right)
      else{
        rotateLeft(p);
        rotateRight(g);
        if(n->getBalance()==-1){
           p->setBalance(0);
           g->setBalance(1);
           n->setBalance(0);
        }
        else if(n->getBalance()==0){
           p->setBalance(0);
           g->setBalance(0);
           n->setBalance(0);
        }
        else if(n->getBalance()==1){
           p->setBalance(-1);
           g->setBalance(0);
           n->setBalance(0);
        }
      }
    }
  }
  //right child
  else{
    g->updateBalance(1);
    if(g->getBalance()==0){
      return;
    }
    else if(g->getBalance()==1){
      insertFix(g,p);
      return;
    }
    else if(g->getBalance()==2){
      //zig-zig (right-right)
      if(p->getRight()==n){
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      //zig-zag (right-left)
      else{
        rotateRight(p);
        rotateLeft(g);
        if(n->getBalance()==1){
           p->setBalance(0);
           g->setBalance(-1);
           n->setBalance(0);
        }
        else if(n->getBalance()==0){
           p->setBalance(0);
           g->setBalance(0);
           n->setBalance(0);
        }
        else if(n->getBalance()==-1){
           p->setBalance(1);
           g->setBalance(0);
           n->setBalance(0);
        }
      }
    }
  }
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    //AVLNode<Key, Value>* prev=NULL;
    AVLNode<Key, Value>* curr=static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::root_);
    //find node
    while(curr!=NULL){
      if(key>(curr->getKey())){
        //prev=curr;
        curr=curr->getRight();
      }
      //if insert is less than go left and progress forward
      else if((key)<(curr->getKey())){
        //prev=curr;
        curr=curr->getLeft();
      }
      else{
        break;
      }
    }
    if(curr==NULL){
      return;
    }  
    //if 2 children
    if(curr->getLeft()!=NULL && curr->getRight()!=NULL){
      AVLNode<Key, Value>* pre=static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::predecessor(curr));
      nodeSwap(curr,pre);
    }
    //one child either right or left
    AVLNode<Key, Value>* c=curr->getLeft();
    if(c==NULL){
      c=curr->getRight();
    }
    //parent of deleted tribe
    AVLNode<Key, Value>* p = curr->getParent();
    if(c!=NULL){
      c->setParent(p);
    }
    int diff=0;
    //+1 from left, -1 from right
    if(p!=NULL){
      if(p->getLeft()==curr){
        p->setLeft(c);
        diff=1;
      }
      else if(p->getRight()==curr){
        p->setRight(c);
        diff=-1;
      }
    }
    //curr is root
    else{
      BinarySearchTree<Key,Value>::root_=c;
    }
    //delete node
    delete curr;
    removeFix(p,diff);
}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff){
  if(n==NULL){
    return;
  }
  AVLNode<Key, Value>* p = n->getParent();
  int ndiff=0;
  if(p!=NULL){
    if(p->getLeft()==n){
      ndiff=1;
    }
    else{
      ndiff=-1;
    }
  }
  if (diff==-1){
    // case1: b(n) + diff == -2
    AVLNode<Key, Value>* c = n->getLeft();
    if(n->getBalance()+diff==-2){
      //zig-zig case
      if(c->getBalance()==-1){
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance()==0){
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else if(c->getBalance()==1){
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance()==1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance()==0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);

        }
        else if(g->getBalance()==-1){
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(n->getBalance()+diff==-1){
      n->setBalance(-1);
    }
    else if(n->getBalance()+diff==0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
  else if (diff==1){
    // case1: b(n) + diff == 2
    AVLNode<Key, Value>* c = n->getRight();
    if(n->getBalance()+diff==2){
      //zig-zig case
      if(c->getBalance()==1){
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance()==0){
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
        return;
      }
      else if(c->getBalance()==-1){
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance()==1){
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance()==0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);

        }
        else if(g->getBalance()==-1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(n->getBalance()+diff==1){
      n->setBalance(1);
    }
    else if(n->getBalance()+diff==0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
