package com.broctagon.trad.reportserver.service;

import java.util.List;

import com.broctagon.trad.reportserver.model.Trade;

public interface MT4TradeService {

	//public void getMT4Trade()
	public void addMT4Trades(List<String> orderList);
	
	
}
