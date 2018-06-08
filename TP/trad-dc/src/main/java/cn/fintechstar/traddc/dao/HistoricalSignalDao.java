package cn.fintechstar.traddc.dao;

import cn.fintechstar.traddc.model.Instrument;

public interface HistoricalSignalDao {

	int getGroupSpread(int groupId, String requestTime, Instrument instrument);
	
}
