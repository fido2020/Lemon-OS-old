#pragma once

template<typename T>
class ListNode
{
	ListNode* next;
	ListNode* prev;
	T obj;
	int index;
};

template<typename T>
class List {
public:
	List();

	void add_back(T obj);
	void add_front(T obj);
	void remove_back();
	void remove_front();
	T get_front();
	T get_back();
	T get_at_index(int index);
	int get_length();
	T  operator[] (unsigned pos);
private:
	ListNode<T>* front;
	ListNode<T>* back;
	int num;
};