package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.GroupSpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.GroupSpreadModel;

@Component
public class GroupSpreadDaoTest {

	@Autowired
	private GroupSpreadDao groupSpreadDao;
	
	public void testAll(){
		testQuery();
		testAdd();
		testQuery();
		testDelete();
	}
	
	public void testQuery(){
		List<GroupSpreadModel> groupSpreadModels = groupSpreadDao.queryGroupSpreadModels();
		if(groupSpreadModels==null){
			System.out.println("groupSpreadModels==null");
		}
		else{
			System.out.println("groupSpreadDao.queryGroupSpreadModels():" + JSON.toJSONString(groupSpreadModels));
		}
		
	}
	
	public void testAdd(){
		GroupSpreadModel groupSpreadModel = new GroupSpreadModel();
		groupSpreadModel.setId(1);
		groupSpreadModel.setGroup_id(1);
		groupSpreadModel.setGroup_name("group_nameTest");
		groupSpreadModel.setSecurity_id(1);
		groupSpreadModel.setSecurity_spread(1);		
		groupSpreadModel.setTime_period("1");
		groupSpreadModel.setMaximum_amounts(1);
		groupSpreadModel.setMinimum_amounts(1);
		groupSpreadModel.setGroup_password("group_passwd_test");
		Float profit = (float)0.1;
		groupSpreadModel.setProfit(profit);
		groupSpreadModel.setOperator_id(1);
		groupSpreadModel.setModify_time("modify_timeTest");
		groupSpreadModel.setDescribe("describeTest");
			
		int oneAddResult = groupSpreadDao.addGroupSpreadModel(groupSpreadModel);
		System.out.println("oneAddResult:" + oneAddResult);
		
/*		List<GroupSpreadModel> symbolSpreadModels = new ArrayList<GroupSpreadModel>();
		symbolSpreadModels.add(groupSpreadModel);
		int multiAddResult = groupSpreadDao.addGroupSpreadModels(symbolSpreadModels);
		System.out.println("multiAddResult:" + multiAddResult);*/
	}
	
	public void testDelete(){
		int deleteResult = groupSpreadDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
}
