package com.broctagon.trad.networkconfiguration.db.dao;

import java.util.List;

import com.broctagon.trad.networkconfiguration.db.model.GroupSpreadModel;

public interface GroupSpreadDao {

	public List<GroupSpreadModel> queryGroupSpreadModels();
	
	public int addGroupSpreadModel(GroupSpreadModel groupSpreadModel);
	public int addGroupSpreadModels(List<GroupSpreadModel> groupSpreadModels);
	
	public int deleteAll();
}
