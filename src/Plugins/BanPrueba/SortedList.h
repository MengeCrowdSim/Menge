#ifndef __SLIST_H__
#define __SLIST_H__

#include <iostream>

using namespace std;

class SortedList {
 private:
  class Node {
   public:
    int info;
    Node* next;
  };

  Node* root;

 public:
  SortedList();
  ~SortedList();
  void insert(int x);
  void print();
};

SortedList::SortedList() { root = NULL; }

SortedList::~SortedList() {
  Node* iter = root;
  Node* del;
  while (iter != NULL) {
    del = iter;
    iter = iter->next;
    delete del;
  }
}

void SortedList::insert(int x) {
  Node* newElement = new Node();
  newElement->info = x;
  if (root == NULL) {
    root = newElement;
  } else {
    if (x < root->info) {
      newElement->next = root;
      root = newElement;
    } else {
      Node* iter = root;
      Node* back = root;
      while (x >= iter->info && iter->next != NULL) {
        back = iter;
        iter = iter->next;
      }
      if (x >= iter->info) {
        iter->next = newElement;
      } else {
        newElement->next = iter;
        back->next = newElement;
      }
    }
  }
}

void SortedList::print() {
  Node* reco = root;
  cout << "Full list.\n";
  while (reco != NULL) {
    cout << reco->info << "-";
    reco = reco->next;
  }
  cout << "\n";
}

int main() {
  SortedList* list = new SortedList();
  list->insert(10);
  list->insert(5);
  list->insert(7);
  list->insert(50);
  list->print();
  delete list;
  return 0;
}

#endif  // __SLIST_H__
