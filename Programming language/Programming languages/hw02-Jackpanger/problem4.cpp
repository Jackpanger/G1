#include <cstdio>

struct Node {
  int data;
  Node* next;
};

void foo(Node* n) {
  delete n->next;
}

Node* create(int x, Node* n) {
  Node* n1 = new Node();
  n1->data = x;
  n1->next = n;

  return n1;
}

int main(void) {
  Node* n1 = create(1, NULL);
  Node* n2 = create(2, n1);

  foo(n2);
  Node* n3 = create(3, NULL);

  n1->data = 42;

  printf("%d\n", n3->data);
  delete(n2);
  delete(n3);

  return 0;
}
