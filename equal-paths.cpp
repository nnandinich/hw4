#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

//function to find depth of each subtree
int helper(Node* root, bool& equal){

    //no nodes so no depth
    //should return true if only no nodes
    if(root==NULL){
        return -1;
    }

    //end of path, so leaf node means depth is 0
    //should return true if only one node
    if((root->right==NULL) && (root->left==NULL)){
        return 0;
    }
    //finding depths of each side
    int leftDepth= helper(root->left, equal);
    int rightDepth= helper(root->right, equal);

    //only left side
    if((root->left!=NULL) && (root->right==NULL)){
        return leftDepth+1;
    }
    //only right side
    if((root->left==NULL) && (root->right!=NULL)){
        return rightDepth+1;
    }
    //if both sides are not equal, then equal is false and no depth
    if(leftDepth!=rightDepth){
        equal=false;
        return -1;
    }
    //else depths of each side are the same, so add one and that is depth of an equal path
    return rightDepth+1;
}

bool equalPaths(Node * root)
{
    // Add your code below
    //start w assumption that paths are equal
    bool equal=true;
    
    //check depths of subtrees
    helper(root, equal);
    return equal;
}

