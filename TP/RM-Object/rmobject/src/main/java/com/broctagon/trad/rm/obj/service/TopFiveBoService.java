/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;




/**
 * @author imdadullah
 *
 */
@Service
public interface TopFiveBoService {

	List<TradeModel> getOpenUpTradeList(List<String> openUpOrderList);

	List<TradeModel> getOpenDownTradeList(List<String> openDownOrderList);

	void writeTop5UpBoToRedis(List<TradeModel> tradeList);

	void writeTop5DownBoToRedis(List<TradeModel> tradeList);
	List<TopFiveBoMsg> processForTop5Bo(List<TradeModel> tradeList);
}
