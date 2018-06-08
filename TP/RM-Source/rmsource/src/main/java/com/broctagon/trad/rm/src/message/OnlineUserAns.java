/**
 * 
 */
package com.broctagon.trad.rm.src.message;

/**
 * @author kanhaiya
 *
 */
public class OnlineUserAns {
	private int clientid;
	private String starttime;
	private String ipaddress;
	
	public int getClientid() {
		return clientid;
	}
	public void setClientid(int clientid) {
		this.clientid = clientid;
	}
	public String getStarttime() {
		return starttime;
	}
	public void setStarttime(String starttime) {
		this.starttime = starttime;
	}
	public String getIpaddress() {
		return ipaddress;
	}
	public void setIpaddress(String ipaddress) {
		this.ipaddress = ipaddress;
	}
}
