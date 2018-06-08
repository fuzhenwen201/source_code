package com.broctagon.trad.rm.obj.service;

import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.message.CustomBoMsg;
import com.broctagon.trad.rm.obj.message.SymbolBoMsg;
import com.broctagon.trad.rm.obj.message.TopFiveBoMsg;

@Service
public interface UpdatedBoPublisherService {

	void updateOpenPositionTbl(List<TradeModel> openTradeListForDB);
	void updateClosedPositionTbl(List<TradeModel> closedPositionList);
	void publishUpdatedTop5BO(List<TopFiveBoMsg> top5UpBo, int direction);
	void publishUpdatedCustomBO(Map<Integer, CustomBoMsg> customBoMap, List<TradeModel> tradList,List<TradeModel> closeTradeList, int direction);
	void publishUpdatedSymbolBO(Map<Integer, SymbolBoMsg> addedSymbolBoMap, List<TradeModel> closeTradeList);
}
