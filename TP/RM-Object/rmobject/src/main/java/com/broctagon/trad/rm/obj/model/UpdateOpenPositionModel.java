/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.List;



/**
 * @author imdadullah
 *
 */
public class UpdateOpenPositionModel {
private List<com.broctagon.trad.rm.db.model.TradeModel> body;

public List<com.broctagon.trad.rm.db.model.TradeModel> getBody() {
	return body;
}

public void setBody(List<com.broctagon.trad.rm.db.model.TradeModel> openTradeList) {
	this.body = openTradeList;
}
}
