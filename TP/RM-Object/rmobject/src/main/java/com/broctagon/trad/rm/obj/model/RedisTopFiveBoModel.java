/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.List;

import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;

/**
 * @author imdadullah
 *
 */
public class RedisTopFiveBoModel {
	String boName;
	List<TopFiveBoMsg> boList;
	public String getBoName() {
		return boName;
	}
	public void setBoName(String boName) {
		this.boName = boName;
	}
	public List<TopFiveBoMsg> getBoList() {
		return boList;
	}
	public void setBoList(List<TopFiveBoMsg> boList) {
		this.boList = boList;
	}
}
