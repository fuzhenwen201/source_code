package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.db.dao.SymbolSpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.SymbolSpreadModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.SymbolSpreadConfigService;

@Service
public class SymbolSpreadConfigServiceImpl implements SymbolSpreadConfigService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private SymbolSpreadDao symbolSpreadDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<SymbolSpreadModel> symbolSpreadModels = JSON.parseArray(configContent, SymbolSpreadModel.class);
		
		if(symbolSpreadModels!=null){
			symbolSpreadDao.deleteAll();
			for(int i=0; i<symbolSpreadModels.size(); i++){
				int addResult = symbolSpreadDao.addSymbolSpreadModel(symbolSpreadModels.get(i));
				logger.info("DB addResult:"+addResult);
			}
		}
		
		rabbitMQHelper.publishSymbolSpreadChangeNotification(CommonConstants.SYMBOL_SPREAD_CHANGE.getBytes());
	}
	
}
