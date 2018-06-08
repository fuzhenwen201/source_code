package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.SgIpUrlMapDao;
import com.broctagon.trad.networkconfiguration.db.model.SgIpUrlMapModel;

@Component
public class SgIpUrlMapDaoTest {

	@Autowired private SgIpUrlMapDao sgIpUrlMapDao;
	
	public void testAll(){
		testQuery();
		testAdd();
		testQuery();
		testDelete();
	}
	
	public void testQuery(){
		List<SgIpUrlMapModel> sgIpUrlMapModels = sgIpUrlMapDao.querySgIpUrlMapModels();
		System.out.println("sgIpUrlMapDao.querySgIpUrlMapModels():" + JSON.toJSONString(sgIpUrlMapModels));
	}
	
	public void testAdd(){
		SgIpUrlMapModel sgIpUrlMapModel = new SgIpUrlMapModel();
		sgIpUrlMapModel.setId(1);
		sgIpUrlMapModel.setSg_internetip("10.10.1");
		sgIpUrlMapModel.setSg_localip("1.1");
		sgIpUrlMapModel.setSg_localport(5005);
		sgIpUrlMapModel.setUrl("www.baidu.com");
		List<SgIpUrlMapModel> addSgIpUrlMapModels = new ArrayList<SgIpUrlMapModel>();
		addSgIpUrlMapModels.add(sgIpUrlMapModel);		
		sgIpUrlMapDao.addSgIpUrlMapModels(addSgIpUrlMapModels);
		
		List<SgIpUrlMapModel> sgIpUrlMapModels = sgIpUrlMapDao.querySgIpUrlMapModels();
		System.out.println("JSON.toJSONString(sgIpUrlMapModels):" + JSON.toJSONString(sgIpUrlMapModels));
		
	}
	
	public void testDelete(){
		int deleteResult = sgIpUrlMapDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
	
}
