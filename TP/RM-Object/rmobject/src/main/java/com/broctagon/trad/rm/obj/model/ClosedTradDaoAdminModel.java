/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.sql.Timestamp;
import java.util.List;

import com.broctagon.trad.rm.db.model.TradeModel;


/**
 * @author imdadullah
 *
 */
public class ClosedTradDaoAdminModel {

Timestamp time;
List<TradeModel> body;
public Timestamp getTimeDuration() {
	return time;
}
public void setTimeDuration(Timestamp time) {
	this.time = time;
}
public List<TradeModel> getBody() {
	return body;
}
public void setBody(List<TradeModel> body) {
	this.body = body;
}

}
