// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment.
// Signed: 조다빈 Student Number: 22000711

// listnode.cpp
//
// Description:	This program implements a simple linked list of nodes
// without a head structure and without sentinel nodes. It simply links
// node to node. The caller is responsible for maintaining the the first
// node (head) of the list. Most functions need the first node to work
// with the list and returns the first node of the list since it may be
// changed inside of the functions.
//
// author: idebtor@gmail.com
//
#include <iomanip>
#include <cstdlib>
#include <stack>
#include "listnode.h"
#include "rand.h"

#if 0
// a basic stack functinality only provided for pedagogical purpose
// To use STL stack, just comment out this section and inclucde <stack> above.
#include <vector>
template <typename T>
struct stack {
	vector<T> data;

	bool empty() const {
		return data.empty();
	}
	auto size() const {
		return data.size();
	}
	void push(T const& item) {
		data.push_back(item);
	}
	void pop() {
		if (data.empty())
			throw out_of_range("stack<>::pop(): pop stack");
		data.pop_back();
	}
	T top() const {
		if (data.empty())
			throw out_of_range("stack<>::top(): top stack");
		return data.back();
	}
};
#endif

// removes all the nodes from the list (which are destroyed),
// and leaving the container nullptr or its size to 0.
pNode clear(pNode p) {
	if (empty(p))
        return nullptr;
	DPRINT(cout << "clear: ";);
	pNode curr = p;
	while (curr != nullptr) {
		pNode prev = curr;
		curr = curr->next;
		DPRINT(cout << prev->data << " ";);
		delete prev;
	}
	cout << "\n\tCleared...Happy Coding!\n";
	return nullptr;
}

// returns the number of nodes in the list.
int size(pNode p) {
	if (empty(p)) return 0;
	int count = 0;
	for (pNode c = p; c != nullptr; c = c->next, count++);
	return count;
}

// sets the min and max values that are passed as references in the list
void minmax(pNode p, int& min, int& max) {
    if (empty(p)) {
        min = max = 0;
        return;
    }
    min = p->data;
    max = p->data;
    while (p != nullptr) {
        if (p->data <= min)
            min = p->data;
        if (p->data >= max)
            max = p->data;
        p = p->next;
    }
    return;
}

// returns the last node of in the list.
pNode last(pNode p) {
	if (empty(p))
        return nullptr;
	while (p->next != nullptr)
        p = p->next;
	return p;
}

// returns true if the list is empty or no nodes.
// To remove all the nodes of a list, see clear().
bool empty(pNode p) {
	return p == nullptr;
}

// inserts a new node with val at the beginning of the list.
// This effectively increases the list size by one.
Node* push_front(Node* p, int val) {
	DPRINT(cout << "><push_front val=" << val << endl;);
	if (empty(p))
        return new Node{ val };
	return new Node{ val, p };
}

// adds a new node with val at the end of the list and returns the
// first node of the list. This effectively increases the list size by one.
pNode push_back(pNode p, int val) {
	DPRINT(cout << "><push_back val=" << val << endl;);
    if (empty(p))
        return new Node{val, nullptr};
    Node *x = last(p);
    x->next = new Node{val, nullptr};
	return p;
}

// inserts a new node with val at the position of the node with x.
// The new node is actually inserted in front of the node with x.
// It returns the first node of the list.
// If the position x is not found, it inserts it at the end of the list.
pNode push(pNode p, int val, int x) {
	if (empty(p))
        return push_front(p, val);
	if (p->data == x)
        return push_front(p, val);

	pNode curr = p;
	pNode prev = nullptr;
    while (curr != nullptr && curr->data != x) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = new Node{val, prev->next};
	// cout << "your code here : use while or for, but do not use if \n";
	return p;
}

// adds N number of new nodes at the front or back of the list.
// Values of new nodes are randomly generated in the range of
// [0..(N + size(p))]. 
// push_fp should be either a function pointer to push_front() 
// or push_back(). Use rand_extended() defined in nowic/include/rand.h.
Node* push_N(Node* p, int N, Node* (*push_fp)(Node*, int)) {
	DPRINT(cout << ">push_N = " << N << endl;);
	int range = N + size(p);
	srand((unsigned)time(nullptr));
    
    for (int i = 0; i < N; i++)
        p = push_fp(p, rand_extended() % (range+1));
    
	DPRINT(cout << "<push_N = " << N << endl;);
	return p;
}

// inserts a new node in sorted ascending order in the list. 
// The basic strategy is to iterate down the list looking for the place to insert 
// the new node. That could be the end of the list, or a point just before a node 
// which is larger than the new node.
Node* push_sorted(Node* p, int val) {
	if (empty(p) || val <= p->data)
        return push_front(p, val);
	Node* curr = p;
	Node* prev = nullptr;
	// locate the node before the point of insertion
    while (curr != nullptr && curr->data <= val) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = new Node{val, curr};
	return p;
}

// sorts the singly linked list using insertion sortand returns a new list sorted.
// Repeatedly, invoke push_sorted() with a value in the list such that push_sorted() 
// returns a newly formed list head.
Node* insertion_sort(Node* p) {
	if (empty(p))
        return nullptr;
	if (size(p) < 2)
        return p;
	Node* sorted = nullptr;
    Node *curr = p;
    while (curr != nullptr) {
        sorted = push_sorted(sorted, curr->data);
        curr = curr->next;
    }
    clear(p);
	return sorted;
}

// removes the first node in the list and returns the new first node.
// This destroys the removed node, effectively reduces its size by one.
pNode pop_front(pNode p) {
	DPRINT(cout << ">pop_front size=" << size(p) << endl;);
    Node *zap = p;
    p = p->next;
    delete zap;
	return p;
}

// removes the last node in the list, effectively reducing the
// container size by one. This destroys the removed node.
pNode pop_back(pNode p) {
	DPRINT(cout << ">pop_back size=" << size(p) << endl;);
	if (empty(p))
        return nullptr;
    pNode curr = p;
    pNode prev = nullptr;
    while (curr->next != nullptr) {
        prev = curr;
        curr = curr->next;
    }
    delete curr;
    if (prev != nullptr)
        prev->next = nullptr;
    else    // p 가 empty 일 때
        p = nullptr;
	DPRINT(cout << "<pop_back size=" << size(p) << endl;);
	return p;
}

// deletes N number of nodes, starting from the beginning or back of the list. 
// It deletes all the nodes if N is zero which is the default or out of 
// the range of the list. 
Node* pop_N(Node* p, int N, Node* (*pop_fp)(Node*)) {
	DPRINT(cout << ">pop_N N=" << N << endl;);
    
    if (N == 0 || N > size(p)){
        while (p !=  nullptr)
            p = pop_fp(p);
        return nullptr;
    }

    for (int i = 0; i < N; i++)
        p = pop_fp(p);

	DPRINT(cout << "<pop_N size=" << size(p) << endl);
	return p;
}

// removes the first occurrence of the node with val from the list
pNode pop(pNode p, int val) {
	DPRINT(cout << ">pop val=" << val << endl;);
	if (empty(p))
        return nullptr;    // nothing to delete
	if (p->data == val)
        return pop_front(p);
	//cout << "your code here : use while or for, but do not use if \n";
    pNode curr = p;
    pNode prev = nullptr;
    while (curr != nullptr && curr->data != val) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != nullptr) {
        prev->next = curr->next;
        delete curr;
    }
	DPRINT(cout << "<pop size=" << size(p) << endl;);
	return p;
}

// reverses a singly linked list using stack and returns the new head. 
// The last node becomes the head node. This function pushes all of its 
// nodes onto a stack. Then top/pop stack and relink the nodes again. 
// It does not create any new node, but recycles and relink them.
// Even though it goes through the list twice, its time complexity is 
// still O(n).This algorithm, however, takes much longer time than 
// in-place reverse algorithm of which the time complexity is also O(n).
Node* reverse_using_stack(Node* head) {
	if (empty(head))
        return nullptr;    // nothing to reverse
	stack<Node*> s;
	Node* curr = head;
    while (!empty(curr)) {
        s.push(curr);
        curr = curr->next;
    }
    curr = s.top();
    s.pop();
    head = curr;
    while (!s.empty()) {
        curr->next = s.top();
        s.pop();
        curr = curr->next;
    }
    curr->next = nullptr;
	return head;
}

// reverses a singly linked list and returns the new head. The last node
// becomes the head node. 
Node* reverse_in_place(Node* head) {
	if (empty(head))
        return nullptr;    // nothing to reverse
	Node* prev = nullptr;
    Node *temp;
	Node* curr = head;
    while (curr != nullptr) {
        temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = temp;
    }
	return prev;
}

// The function cut_in_two_halves() cuts the list in two halves. 
// It returns the first node of the first half and the first node of the 
// second half of the list. If there are odd number of nodes, the second 
// half keeps one more node than the first half. For example, 
// the second half keeps 5 nodes if there are 9 nodes.
// the size of the list must be equal to or greater than two.
pair<Node*, Node*> cut_in_two_halves(Node* p) {
    /* driver.cpp 에서 empty, size<2 일 경우 break 하게 했으므로, 주석 처리함
    if (empty(p))
        return make_pair(nullptr, nullptr);
    if (size(p) < 2)
        return make_pair(p, p);
     */
    int n = size(p) / 2;
    int count = 0;
    Node *pmid = p;
    
    for (count = 0; count < n - 1; count++)
        pmid = pmid->next;
    Node *mid = pmid->next;
    pmid->next = nullptr;
	return make_pair(p, mid);
}

// removes consecutive items in the list, and leaves its neighbors unique.
// We can proceed down the listand compare adjacent nodes. When adjacent 
// nodes are the same, remove the second one.There's a tricky case where 
// the node after the next node needs to be noted before the deletion. 
// Your implementation must go through the list only once.
Node* zap_duplicates(Node* p) {
	if (empty(p))
        return nullptr;
    Node *curr = p;
    Node *zap;
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->data == curr->next->data) {
            zap = curr->next;
            curr->next = curr->next->next;
            delete zap;
        }
        else
            curr = curr->next;
    }
	return p;
}

// shows the values of all the nodes in the list if all is true or
// the list size is less than or equal to show_n * 2. If there are more than
// (show_n * 2) nodes, then it shows only show_n number of nodes from
// the beginning and the end in the list.
void show(pNode p, bool all, int show_n) {
    DPRINT(cout << "show(" << size(p) << ")\n";);
    if (empty(p)) {
        cout << "\n\tThe list is empty.\n";
        return;
    }

    int i;
    pNode curr;
    const int N = size(p);
    cout << endl << "TOP ";
    
    if (all || N <= show_n * 2) {
        for (i = 1, curr = p; curr != nullptr; curr = curr->next, i++) {
            cout << "\t" << curr->data;
            if (i % show_n == 0)
                cout << endl;
        }
        return;
    }
    
    // print the first show_n items
    for (i = 1, curr = p; curr != nullptr && i <= show_n; curr = curr->next, i++) {
        cout << "\t" << curr->data;
    }

    cout << "\n...left out...\n";

    // print the last show_n items
    // Firstly, move the pointer to the place where show_n items are left.
    for (curr = p; curr != nullptr && size(curr) != show_n;)
        curr = curr->next;
    while (curr != nullptr) {
        cout << "\t" << curr->data;
        curr = curr->next;
    }

    cout << endl;
}
