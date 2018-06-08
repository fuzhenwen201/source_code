/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.List;

import com.broctagon.trad.rm.db.model.Order;



/**
 * @author imdadullah
 *
 */
public class UpdateOpenPositionModel {
private List<Order> body;

public List<Order> getBody() {
	return body;
}

public void setBody(List<Order> openTradeList) {
	this.body = openTradeList;
}
}
