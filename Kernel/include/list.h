#include <stdtype.h>

struct ListNode{
	void* object;
	struct ListNode* next;
	struct ListNode* prev;
};

class List{
	public:
		List();
		
		void Add(void* object);
		
		void* RetrieveAt(uint32_t index);
		void* RetrieveLast();
		uint32_t GetItemCount();
	private:
		uint32_t count;
		ListNode* rootNode;
	
};