/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
@Service
public interface TraderHistoryService {
	void tradersHistory(Message message,int reqTag);
	List<Order> getOpenClosedTradeList(int clientId);
	void updateTraderHistory(List<TradeModel> closeTradeList);
}
