package com.broctagon.trad.networkconfiguration.db.daotest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.SymbolSpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.SymbolSpreadModel;

@Component
public class SymbolSpreadDaoTest {

	@Autowired
	private SymbolSpreadDao symbolSpreadDao;
	
	public void testAll(){
		testQuery();
		testAdd();
		testQuery();
		testDelete();
	}
	
	public void testQuery(){
		List<SymbolSpreadModel> symbolSpreadModels = symbolSpreadDao.querySymbolSpreadModels();
		System.out.println("symbolSpreadDao.querySymbolSpreadModels():" + JSON.toJSONString(symbolSpreadModels));
	}
	
	public void testAdd(){
		SymbolSpreadModel symbolSpreadModel = new SymbolSpreadModel();
		symbolSpreadModel.setId(1);
		symbolSpreadModel.setSymbol("symbolTest");
		symbolSpreadModel.setSymbol_spread(1);
		symbolSpreadModel.setOperator_id(1);
		symbolSpreadModel.setModify_time("modify_timeTest");
		symbolSpreadModel.setDescribe("describeTest");
		
		int oneAddResult = symbolSpreadDao.addSymbolSpreadModel(symbolSpreadModel);
		System.out.println("oneAddResult:" + oneAddResult);
		
		List<SymbolSpreadModel> symbolSpreadModels = new ArrayList<SymbolSpreadModel>();
		symbolSpreadModels.add(symbolSpreadModel);
		int multiAddResult = symbolSpreadDao.addSymbolSpreadModels(symbolSpreadModels);
		System.out.println("multiAddResult:" + multiAddResult);
	}
	
	public void testDelete(){
		int deleteResult = symbolSpreadDao.deleteAll();
		System.out.println("deleteResult:" + deleteResult);
	}
}
