package com.broctagon.centrallogging.util;

public class CircleArray<T> {

	private T[] items;
	private int head, tail, count = 0;
	
	@SuppressWarnings("unchecked")
	public CircleArray(int size){
		items = (T[]) new Object[size];
	}
	
	public boolean isEmpty(){
		return count == 0;
	}
	
	public void set(T item){
		if(tail == items.length){
			tail = 0;
		}
		items[tail++] = item;
		if(count != items.length){
			count ++;	
		}
	}
	
	public T get(){
		if (isEmpty()) {
			return null;
		}
		count --;
		return items[head++];
	}
	
	public Object[] getAll(){
		Object[] clone = new Object[count];
		int index = 0;
		for(int i = head ; i < count; i ++){
			if(head == items.length){
				head = 0;
				count = tail;
			}
			clone[index++] = items[i];
		}
		return clone;
	}
}