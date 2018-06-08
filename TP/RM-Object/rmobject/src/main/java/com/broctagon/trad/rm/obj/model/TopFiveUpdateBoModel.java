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
public class TopFiveUpdateBoModel {

	private int direction;
	private List<TopFiveBoMsg> models;
	
	public List<TopFiveBoMsg> getModels() {
		return models;
	}
	public void setModels(List<TopFiveBoMsg> models) {
		this.models = models;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	
}
