/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.Map;

import com.broctagon.trad.rm.obj.message.SymbolBoMsg;

/**
 * @author imdadullah
 *
 */
public class RedisSymbolBoModel {
	String boName;
	Map<Integer, SymbolBoMsg> boMap;
	public String getBoName() {
		return boName;
	}
	public void setBoName(String boName) {
		this.boName = boName;
	}
	public Map<Integer, SymbolBoMsg> getBoMap() {
		return boMap;
	}
	public void setBoMap(Map<Integer, SymbolBoMsg> boMap) {
		this.boMap = boMap;
	}
}
