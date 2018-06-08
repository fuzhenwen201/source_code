package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import javax.annotation.PostConstruct;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.BoCalculationDao;
import com.broctagon.trad.networkconfiguration.db.dao.GroupBoSymbolsDao;
import com.broctagon.trad.networkconfiguration.db.dao.GroupSpreadDao;
import com.broctagon.trad.networkconfiguration.db.dao.SecuritySpreadDao;
import com.broctagon.trad.networkconfiguration.db.dao.SymbolSpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.BoCalculationModel;
import com.broctagon.trad.networkconfiguration.db.model.GroupBoSymbolsModel;
import com.broctagon.trad.networkconfiguration.db.model.GroupSpreadModel;
import com.broctagon.trad.networkconfiguration.db.model.SecuritySpreadModel;
import com.broctagon.trad.networkconfiguration.db.model.SymbolSpreadModel;
import com.broctagon.trad.networkconfiguration.service.AdminConfigService;
import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperConstants;
import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperHelper;

@Service
public class AdminConfigServiceImpl implements AdminConfigService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private ZookeeperHelper zkHelper;
	@Autowired private BoCalculationDao boCalculationDao;
	@Autowired private GroupBoSymbolsDao groupBoSymbolsDao;
	@Autowired private GroupSpreadDao groupSpreadDao;
	@Autowired private SecuritySpreadDao securitySpreadDao;
	@Autowired private SymbolSpreadDao symbolSpreadDao;
	
	
	@PostConstruct
	public void updateAll(){
		logger.info("updateAll");
		updateBoCalculation();
		updateGroupBoSymbols();
		updateGroupSpread();
		updateSecuritySpread();
		updateSymbolSpread();
	}
	
	public void updateBoCalculation(){
		List<BoCalculationModel> allModels = boCalculationDao.queryBoCalculationModels();
		String allData = JSON.toJSONString(allModels);
		logger.info("allData:" + allData);
		try {
			zkHelper.setData(ZookeeperConstants.ZK_PATH_BOCALCULATION, allData.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void updateGroupBoSymbols(){
		List<GroupBoSymbolsModel> allModels = groupBoSymbolsDao.queryGroupBoSymbolsModels();
		String allData = JSON.toJSONString(allModels);
		logger.info("allData:" + allData);
		try {
			zkHelper.setData(ZookeeperConstants.ZK_PATH_GROUPBOSYMBOL, allData.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void updateGroupSpread(){
		List<GroupSpreadModel> allModels = groupSpreadDao.queryGroupSpreadModels();
		String allData = JSON.toJSONString(allModels);
		logger.info("allData:" + allData);
		try {
			zkHelper.setData(ZookeeperConstants.ZK_PATH_GROUPSPREAD, allData.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void updateSecuritySpread(){
		List<SecuritySpreadModel> allModels = securitySpreadDao.querySecuritySpreadModels();
		String allData = JSON.toJSONString(allModels);
		logger.info("allData:" + allData);
		try {
			zkHelper.setData(ZookeeperConstants.ZK_PATH_SECURITYSPREAD, allData.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
		
	public void updateSymbolSpread(){
		List<SymbolSpreadModel> allModels = symbolSpreadDao.querySymbolSpreadModels();
		String allData = JSON.toJSONString(allModels);
		logger.info("allData:" + allData);
		try {
			zkHelper.setData(ZookeeperConstants.ZK_PATH_SYMBOLSPREAD, allData.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
