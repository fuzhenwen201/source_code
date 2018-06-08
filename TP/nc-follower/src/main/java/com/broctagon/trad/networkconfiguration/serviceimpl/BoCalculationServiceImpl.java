package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.common.CommonUtil;
import com.broctagon.trad.networkconfiguration.db.dao.BoCalculationDao;
import com.broctagon.trad.networkconfiguration.db.model.BoCalculationModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.BoCalculationService;

@Service
public class BoCalculationServiceImpl implements BoCalculationService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private BoCalculationDao boCalculationDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<BoCalculationModel> boCalculationModel = JSON.parseArray(configContent, BoCalculationModel.class);
		
		if(boCalculationModel!=null){
			boCalculationDao.deleteAll();
			for(int i=0; i<boCalculationModel.size(); i++){
				BoCalculationModel model = boCalculationModel.get(i);
				String symbol_list = model.getSymbol_list();
				logger.info("symbol_list:" + symbol_list);
				if(!CommonUtil.isEmptyString(symbol_list)){
					if(symbol_list.endsWith(";")){
						model.setSymbol_list(symbol_list.substring(0, symbol_list.length()-1));
					}	
					int addResult = boCalculationDao.addBoCalculationModel(model);
					logger.info("DB addResult:"+addResult);					
				}
			}
		}
		
		rabbitMQHelper.publishBoSymbolChangeNotification(CommonConstants.BOSYMBOL_CHANGE.getBytes());
		rabbitMQHelper.publishBoSymbolChangeNotification_Old(CommonConstants.BOSYMBOL_CHANGE.getBytes());
	}
	
}
