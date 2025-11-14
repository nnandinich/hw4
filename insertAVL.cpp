#include <iostream>
#include "avlbst.h"
using namespace std;

int main(){
  AVLTree<char,int> bt;
  cout<<"inserting a,1" << endl;
  bt.insert(make_pair('a',1));
  cout<<"inserting b,2" << endl;
  bt.insert(make_pair('b',2));
  cout<<"inserting c,3" << endl;
  bt.insert(make_pair('c',3));
  cout<<"inserting d,4" << endl;
  bt.insert(make_pair('d',4));
  cout <<"after inserts "<< endl;
  bt.print();

  return 0;
}