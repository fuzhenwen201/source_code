package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.common.CommonUtil;
import com.broctagon.trad.networkconfiguration.db.dao.SecuritySpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.SecuritySpreadModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.SecuritySpreadService;

@Service
public class SecuritySpreadServiceImpl implements SecuritySpreadService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private SecuritySpreadDao securitySpreadDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<SecuritySpreadModel> securitySpreadModels = JSON.parseArray(configContent, SecuritySpreadModel.class);
		
		if(securitySpreadModels!=null){
			securitySpreadDao.deleteAll();
			for(int i=0; i<securitySpreadModels.size(); i++){
				SecuritySpreadModel model = securitySpreadModels.get(i);
				String symbol_list = model.getSymbol_list();
				logger.info("symbol_list:" + symbol_list);
				if(!CommonUtil.isEmptyString(symbol_list)){
					if(symbol_list.endsWith(";")){
						model.setSymbol_list(symbol_list.substring(0, symbol_list.length()-1));
					}					
					int addResult = securitySpreadDao.addSecuritySpreadModel(model);
					logger.info("DB addResult:"+addResult);
				}	
							
			}
		}
		
		rabbitMQHelper.publishGroupSpreadChangeNotification(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
		rabbitMQHelper.publishGroupSpreadChangeNotification_Old(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
	}
	
}
