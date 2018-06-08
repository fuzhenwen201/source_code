package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.SecuritySpreadModel;

public interface SecuritySpreadDao {
	
	public List<SecuritySpreadModel> querySecuritySpreadModels();
	
	public int addSecuritySpreadModel(SecuritySpreadModel securitySpreadModel);
	public int addSecuritySpreadModels(List<SecuritySpreadModel> securitySpreadModels);
	
	public int deleteAll();
}
