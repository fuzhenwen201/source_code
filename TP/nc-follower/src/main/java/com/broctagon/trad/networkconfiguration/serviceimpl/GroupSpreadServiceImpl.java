package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.db.dao.GroupSpreadDao;
import com.broctagon.trad.networkconfiguration.db.model.GroupSpreadModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.GroupSpreadService;

@Service
public class GroupSpreadServiceImpl implements GroupSpreadService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private GroupSpreadDao groupSpreadDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<GroupSpreadModel> groupSpreadModels = JSON.parseArray(configContent, GroupSpreadModel.class);
		
		if(groupSpreadModels!=null){
			groupSpreadDao.deleteAll();
			for(int i=0; i<groupSpreadModels.size(); i++){
				int addResult = groupSpreadDao.addGroupSpreadModel(groupSpreadModels.get(i));
				logger.info("DB addResult:"+addResult);
			}
		}
		
		rabbitMQHelper.publishGroupSpreadChangeNotification(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());	
		rabbitMQHelper.publishGroupSpreadChangeNotification_Old(CommonConstants.GROUP_SPREAD_CHANGE.getBytes());
	}
	
}
