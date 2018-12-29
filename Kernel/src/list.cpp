#include <list.h>

#include <memory.h>

/*template<typename T>
List<T>::List()
{
}

template<typename T>
void List<T>::add_back(T obj) {
	ListNode<T>* node = malloc(sizeof(ListNode<T>));
	node.obj = obj;
	node.index = num;
	node.prev = back;
	node.next = nullptr;
	back->next = &node;
	back = &node;
	num++;
}

template<typename T>
T List<T>::get_at_index(int index) {
	ListNode<T>* node = front;
	while (node->index != index && node->next != nullptr) {
		node = node->next;
	}
	return node->obj;
}

template<typename T>
T List<T>::operator[](unsigned pos) {
	return get_at_index(pos);
}

template<typename T>
int List<T>::get_length() {
	return num;
}

template<typename T>
void List<T>::add_front(T obj)
{
}

template<typename T>
void List<T>::remove_back()
{
}

template<typename T>
void List<T>::remove_front()
{
}

template<typename T>
T List<T>::get_front()
{
	return front->obj;
}

template<typename T>
T List<T>::get_back()
{
	return back->obj;
}*/