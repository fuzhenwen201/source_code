package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.db.dao.GroupBoSymbolsDao;
import com.broctagon.trad.networkconfiguration.db.model.GroupBoSymbolsModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.GroupBoSymbolService;

@Service
public class GroupBoSymbolServiceImpl implements GroupBoSymbolService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private GroupBoSymbolsDao groupBoSymbolsDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<GroupBoSymbolsModel> groupBoSymbolsModels = JSON.parseArray(configContent, GroupBoSymbolsModel.class);
		
		if(groupBoSymbolsModels!=null){
			groupBoSymbolsDao.deleteAll();
			for(int i=0; i<groupBoSymbolsModels.size(); i++){
				int addResult = groupBoSymbolsDao.addGroupBoSymbolsModel(groupBoSymbolsModels.get(i));
				logger.info("DB addResult:"+addResult);
			}
		}
		
		rabbitMQHelper.publishBoSymbolChangeNotification(CommonConstants.BOSYMBOL_CHANGE.getBytes());
		rabbitMQHelper.publishBoSymbolChangeNotification_Old(CommonConstants.BOSYMBOL_CHANGE.getBytes());
	}
	
}
