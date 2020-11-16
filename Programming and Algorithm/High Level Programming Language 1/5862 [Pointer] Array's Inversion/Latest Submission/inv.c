void inv(int* array,int size){
	int i;
	for(i = 0;i < size-i-1;i++){
		int temp = array[i];
		array[i] = array[size-i-1];
		array[size-i-1] = temp;
	}
}