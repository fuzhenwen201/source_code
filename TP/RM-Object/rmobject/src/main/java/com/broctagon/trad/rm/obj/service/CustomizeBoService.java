/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.message.CustomBoMsg;



/**
 * @author imdadullah
 *
 */
@Service
public interface CustomizeBoService {
	void writeCustomizedUpBoToRedis(List<TradeModel>openUpTradeList,List<TradeModel>openUpListForUpdate,List<TradeModel>closeTradeList);
	void writeCustomizedDownBoToRedis(List<TradeModel>openDownTradeList,List<TradeModel>openDownListForUpdate,List<TradeModel>closeTradeList);
	Map<Integer, CustomBoMsg> processForCustomizedBo(List<TradeModel> tradeList,int direction);
}

