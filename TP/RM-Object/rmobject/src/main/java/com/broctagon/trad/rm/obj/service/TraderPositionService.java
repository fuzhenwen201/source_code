/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
@Service
public interface TraderPositionService {
	void tradersPosition(Message message,int reqTag);
	List<TradeModel> getOpenTradeList(int clientId);
	void updateTraderOpenPosition(List<TradeModel> openTradeList);
	void updateTraderClosedPosition(List<TradeModel> closeTradeList);
}
