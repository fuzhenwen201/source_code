
package com.broctagon.trad.rm.src.service;

/**
* @auther: Water
* @time: Oct 9, 2017 5:48:29 PM
* 
*/

public interface OnlineUserSetService {

	public void addOnlineUser(byte[] content);
	public void removeOnlineUser(byte[] content);
	
	public void requestAllOnlineUsers();
	public void storeAllOnlineUsers(byte[] content);
	
}
