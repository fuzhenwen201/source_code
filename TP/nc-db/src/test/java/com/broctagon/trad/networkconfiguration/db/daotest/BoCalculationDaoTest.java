package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.BoCalculationDao;
import com.broctagon.trad.networkconfiguration.db.model.BoCalculationModel;


@Component
public class BoCalculationDaoTest {

	@Autowired
	private BoCalculationDao boCalculationDao;
	
	public void testAll(){
		testQuery();
		testDelete();
		testAdd();
		testQuery();
	}
	
	public void testQuery(){
		List<BoCalculationModel> boCalculationModels = boCalculationDao.queryBoCalculationModels();
		System.out.println("boCalculationDao.queryBoCalculationModels():" + JSON.toJSONString(boCalculationModels));
	}
	
	public void testAdd(){
		BoCalculationModel boCalculationModel = new BoCalculationModel();
		boCalculationModel.setId(1);
		
		boCalculationModel.setSecurity_id(1);
		boCalculationModel.setSecurity_name("security_name_test");
		boCalculationModel.setSymbol_list("symbol_list_test");
		
		boCalculationModel.setOperator_id(1);
		boCalculationModel.setModify_time("modify_timeTest");
		boCalculationModel.setDescribe("describeTest");
		
		int oneAddResult = boCalculationDao.addBoCalculationModel(boCalculationModel);
		System.out.println("oneAddResult:" + oneAddResult);
		
		boCalculationModel.setId(3);
		List<BoCalculationModel> boCalculationModels = new ArrayList<BoCalculationModel>();
		boCalculationModels.add(boCalculationModel);
		int multiAddResult = boCalculationDao.addBoCalculationModels(boCalculationModels);
		System.out.println("multiAddResult:" + multiAddResult);
	}
	
	public void testDelete(){
		int deleteResult = boCalculationDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
	
}
