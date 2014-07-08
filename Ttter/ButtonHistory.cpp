#include "Arduino.h"
#include "ButtonHistory.h"

ButtonHistory::ButtonHistory(void){
	size = 50;
	history[size];
	head = size-1; //999
	tail = size-2; //998
}

int ButtonHistory::add(int flag){
	head++;
	tail++;
	if(head >= size){ //init => 0
		head=0;
	}
	if(tail >= size){ //init =>999
		tail=0;
	}
	history[head] = flag;
	return flag;
}

/*
bool ButtonHistory::show(void){
	for(int i=0; i<size; i++){
		int tmp = head+i;
		if(tmp>=size){// ex10000
			tmp -= size;
		}
		printf("%s ", history[tmp]);
	}
	return true;
}*/
/*
int *ButtonHistory::get_all(void){
	int tmp_history[size];
	for(int i=0; i<size; i++){
		int tmp = head+i;
		if(tmp>=size){// ex10000
			tmp -= size;
		}
		tmp_history[i] = history[tmp];
	}
	return tmp_history;
}*/

short ButtonHistory::get_by_index(int index){
	short tmp=head+index;
	if(tmp>=size){
		tmp -= size;
	}
	return tmp;
}

short ButtonHistory::get_size(void){
	return size;
}