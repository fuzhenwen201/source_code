package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.GroupBoSymbolsDao;
import com.broctagon.trad.networkconfiguration.db.model.GroupBoSymbolsModel;


@Component
public class GroupBoSymbolsDaoTest {

	@Autowired
	private GroupBoSymbolsDao groupBoSymbolsDao;
	
	public void testAll(){
		testQuery();
		testAdd();
		testQuery();
		testDelete();
	}
	
	public void testQuery(){
		List<GroupBoSymbolsModel> groupBoSymbolsModels = groupBoSymbolsDao.queryGroupBoSymbolsModels();
		System.out.println("groupBoSymbolsDao.querygroupBoSymbolsModels():" + JSON.toJSONString(groupBoSymbolsModels));
	}
	
	public void testAdd(){
		
		GroupBoSymbolsModel groupBoSymbolsModel = new GroupBoSymbolsModel();
		groupBoSymbolsModel.setId(1);
		groupBoSymbolsModel.setGroup_id(1);
		groupBoSymbolsModel.setGroup_name("group_nameTest");
		groupBoSymbolsModel.setSecurity_id(1);
		groupBoSymbolsModel.setBo_caltype(1);		
		groupBoSymbolsModel.setTime_period("1");
		groupBoSymbolsModel.setMaximum_amounts(1);
		groupBoSymbolsModel.setMinimum_amounts(1);
		groupBoSymbolsModel.setGroup_password("group_passwd_test");
		Float profit = (float)0.1;
		groupBoSymbolsModel.setProfit(profit);
		groupBoSymbolsModel.setOperator_id(1);
		groupBoSymbolsModel.setModify_time("modify_timeTest");
		groupBoSymbolsModel.setDescribe("describeTest");
			
		int oneAddResult = groupBoSymbolsDao.addGroupBoSymbolsModel(groupBoSymbolsModel);
		System.out.println("oneAddResult:" + oneAddResult);
		
		List<GroupBoSymbolsModel> groupBoSymbolsModels = new ArrayList<GroupBoSymbolsModel>();
		groupBoSymbolsModels.add(groupBoSymbolsModel);
		int multiAddResult = groupBoSymbolsDao.addGroupBoSymbolsModels(groupBoSymbolsModels);
		System.out.println("multiAddResult:" + multiAddResult);
	}
	
	public void testDelete(){
		int deleteResult = groupBoSymbolsDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
	
}
