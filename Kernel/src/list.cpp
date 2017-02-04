#include <common.h>

#include <list.h>

List::List(){
	count = 0;
	rootNode = (ListNode*)0;
}

void List::Add(void* _object){
	ListNode* node;
	node = (ListNode*)malloc(sizeof(ListNode));
	
	if(!rootNode){
		rootNode = node;
	} else {
		ListNode* currentNode = rootNode;
		
		while(currentNode->next)
			currentNode = currentNode->next;
	
		currentNode->next = node;
		node->prev = currentNode;
	}
	
	count++;
}

void* List::RetrieveAt(uint32_t index){
	if(count == 0)
		return nullptr;
	
	ListNode* currentNode = rootNode;
	
	for(uint32_t i=0; (i < index) && currentNode;i++)
		currentNode = currentNode->next;
	
	return currentNode->object;
}

void* List::RetrieveLast(){
	return RetrieveAt(count);
}

uint32_t List::GetItemCount(){ return count; }