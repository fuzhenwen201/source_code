package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.SgIpUrlMapModel;

public interface SgIpUrlMapDao {

	public List<SgIpUrlMapModel> querySgIpUrlMapModels();
	
	public int addSgIpUrlMapModel(SgIpUrlMapModel sgIpUrlMapModel);
	public int addSgIpUrlMapModels(List<SgIpUrlMapModel> sgIpUrlMapModels);
	
	public int deleteAll();
}
