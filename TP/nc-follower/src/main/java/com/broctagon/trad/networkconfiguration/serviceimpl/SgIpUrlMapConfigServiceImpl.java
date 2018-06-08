package com.broctagon.trad.networkconfiguration.serviceimpl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonConstants;
import com.broctagon.trad.networkconfiguration.db.dao.SgIpUrlMapDao;
import com.broctagon.trad.networkconfiguration.db.model.SgIpUrlMapModel;
import com.broctagon.trad.networkconfiguration.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.networkconfiguration.service.SgIpUrlMapConfigService;

@Service
public class SgIpUrlMapConfigServiceImpl implements SgIpUrlMapConfigService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private SgIpUrlMapDao sgIpUrlMapDao;
	@Autowired private RabbitMQHelper rabbitMQHelper; 
	
	@Transactional
	public void updateConfigAndNotify(byte[] data){		
		String configContent = new String(data);
		List<SgIpUrlMapModel> sgIpUrlMapModels = JSON.parseArray(configContent, SgIpUrlMapModel.class);
		
		if(sgIpUrlMapModels!=null){
			  	sgIpUrlMapDao.deleteAll();
			for(int i=0; i<sgIpUrlMapModels.size(); i++){
				int addResult = sgIpUrlMapDao.addSgIpUrlMapModel(sgIpUrlMapModels.get(i));
				logger.info("DB addResult:"+addResult);
			}
		}
		
		rabbitMQHelper.publishUrlMapChangeNotification(CommonConstants.URL_MAP_CHANGE.getBytes());
	}
	
}
