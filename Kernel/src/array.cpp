#include <array.h>

namespace array{
	auto* append(auto* array, auto data){
		
		for(int i = 0; i < (sizeof(array) / sizeof(data)); i++){
			if(array[i] == nullptr){
				array[i] = data;
				break;
			}
		}
		
		return array;
	}
}