/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.message.SymbolBoMsg;




/**
 * @author imdadullah
 *
 */
@Service
public interface SymbolBoService {
	
	List<TradeModel> getClosedTradeList(List<String> closedOrderList);
	void writeSymbolBoToRedis(List<TradeModel> tradeList);
	Map<Integer, SymbolBoMsg> processForSymbolBo(List<TradeModel> tradeList);

}
