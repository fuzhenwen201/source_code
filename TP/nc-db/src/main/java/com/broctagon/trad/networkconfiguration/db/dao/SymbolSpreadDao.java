package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.SymbolSpreadModel;


public interface SymbolSpreadDao {
	
	public List<SymbolSpreadModel> querySymbolSpreadModels();
	
	public int addSymbolSpreadModel(SymbolSpreadModel symbolSpreadModel);
	public int addSymbolSpreadModels(List<SymbolSpreadModel> symbolSpreadModels);
	
	public int deleteAll();
}
