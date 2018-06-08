/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.Map;

import com.broctagon.trad.rm.obj.message.CustomBoMsg;
/**
 * @author imdadullah
 *
 */
public class RedisCustomizedBoModel {
	String boName;
	Map<Integer, CustomBoMsg> boMap;
	public String getBoName() {
		return boName;
	}
	public void setBoName(String boName) {
		this.boName = boName;
	}
	public Map<Integer, CustomBoMsg> getBoMap() {
		return boMap;
	}
	public void setBoMap(Map<Integer, CustomBoMsg> boMap) {
		this.boMap = boMap;
	}


}
