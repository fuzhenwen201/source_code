package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.GroupBoSymbolsModel;

public interface GroupBoSymbolsDao {
	
	public List<GroupBoSymbolsModel> queryGroupBoSymbolsModels();
	
	public int addGroupBoSymbolsModel(GroupBoSymbolsModel groupBoSymbolsModel);
	public int addGroupBoSymbolsModels(List<GroupBoSymbolsModel> groupBoSymbolsModels);
	
	public int deleteAll();
}
