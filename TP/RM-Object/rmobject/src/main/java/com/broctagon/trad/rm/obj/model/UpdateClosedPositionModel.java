/**
 * 
 */
package com.broctagon.trad.rm.obj.model;

import java.util.List;

import com.broctagon.trad.rm.obj.message.UpdateClosedPositionMsg;


/**
 * @author imdadullah
 *
 */
public class UpdateClosedPositionModel {
private List<UpdateClosedPositionMsg> body;

public List<UpdateClosedPositionMsg> getBody() {
	return body;
}
public void setBody(List<UpdateClosedPositionMsg> body) {
	this.body = body;
}

}
