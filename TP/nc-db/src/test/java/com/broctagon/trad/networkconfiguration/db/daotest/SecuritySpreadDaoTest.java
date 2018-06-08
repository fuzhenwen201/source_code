package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.SecuritySpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.SecuritySpreadModel;


@Component
public class SecuritySpreadDaoTest {

	@Autowired
	private SecuritySpreadDao securitySpreadDao;
	
	public void testAll(){
		testQuery();
		testDelete();
		testAdd();
		testQuery();
	}
	
	public void testQuery(){
		List<SecuritySpreadModel> securitySpreadModels = securitySpreadDao.querySecuritySpreadModels();
		System.out.println("securitySpreadDao.querySecuritySpreadModels():" + JSON.toJSONString(securitySpreadModels));
	}
	
	public void testAdd(){
		SecuritySpreadModel securitySpreadModel = new SecuritySpreadModel();
		securitySpreadModel.setId(1);
		securitySpreadModel.setSecurity_id(1);
		securitySpreadModel.setSecurity_name("Security_nameTest");
		securitySpreadModel.setSymbol_list("symbo_ListTest");
		securitySpreadModel.setOperator_id(1);
		securitySpreadModel.setModify_time("modify_timeTest");
		securitySpreadModel.setDescribe("describeTest");
		
		int oneAddResult = securitySpreadDao.addSecuritySpreadModel(securitySpreadModel);
		System.out.println("oneAddResult:" + oneAddResult);
		
		securitySpreadModel.setId(3);
		List<SecuritySpreadModel> securitySpreadModels = new ArrayList<SecuritySpreadModel>();
		securitySpreadModels.add(securitySpreadModel);
		int multiAddResult = securitySpreadDao.addSecuritySpreadModels(securitySpreadModels);
		System.out.println("multiAddResult:" + multiAddResult);
	}
	
	public void testDelete(){
		int deleteResult = securitySpreadDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
	
}
