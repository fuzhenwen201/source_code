/**
 * 
 */
package com.broctagon.trad.rm.db.test;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.SpringBootApp;
import com.broctagon.trad.rm.db.dao.OpenTradeDao;
import com.broctagon.trad.rm.db.model.TradeModel;


/**
 * @author imdadullah
 *
 */
@RunWith(SpringRunner.class)
@SpringBootTest(classes=SpringBootApp.class)
public class DbOpenTradDaoTest{
	private Logger logger = Logger.getLogger(DbOrderDaoTest.class);
	@Autowired
	private OpenTradeDao openTradDao;
	List<TradeModel> tradList = new ArrayList<TradeModel>();
	public String orderId;
	
	public List<TradeModel> tradModelList(){
	for(int i=0; i<2; i++){			
		TradeModel trad = new TradeModel();
		trad.setReqid("4e978baf75d54a5d8bca9c0491f9ff9d");
		trad.setGroupid(1);
		trad.setClientid(83);
		trad.setOrderstatus(0);
		trad.setAssetid(1);
		trad.setDirection(0);
		trad.setAmount(100);
		trad.setExpiryperiod(0);
		long opentime = System.currentTimeMillis();
		trad.setOpentime(opentime);
		trad.setOpenprice(729000);
		trad.setCloseprice(72);
		trad.setPayout(0);
		trad.setSgid(1);
		trad.setTeid(0);
		trad.setExpirytime(opentime+15000);
		trad.setComment("comment test");
		trad.setType(4);
		UUID uuid = UUID.randomUUID();
		String uuidStr = uuid.toString().replaceAll("-", "");	
		trad.setOrderid(uuidStr);
		String orderStr = JSON.toJSONString(trad);
		logger.info("openTradStr:"+orderStr);
					
		tradList.add(trad);	
		orderId = uuidStr;
		}
	return tradList;
	}
	
	//@Test
	public void insertOpenPosition(){
	//	List<TradeModel> tradList = tradModelList();
		int result = openTradDao.insertOpenTrades(tradList);
	//	int result = openTradDao.insertOpenTrade(trad);
		logger.info("Result of insertion in openposition" +result);
	}
	@Test
	public void selectOpenPosition(){
		List<TradeModel> tradList = openTradDao.selectAllOpenTrade();
		logger.info("Query all Openposition" +JSON.toJSONString(tradList));
		
		List<TradeModel> tradByClient = openTradDao.selectOpenTradeByClientid(83);
		logger.info("Query Openposition By Client Id " +JSON.toJSONString(tradByClient));
	}
	@Test
	public void deleteOpenPosition(){
		openTradDao.deleteOpenTrade(orderId);
	}
}
