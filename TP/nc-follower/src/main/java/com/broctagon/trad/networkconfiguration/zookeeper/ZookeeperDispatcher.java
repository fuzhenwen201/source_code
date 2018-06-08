
package com.broctagon.trad.networkconfiguration.zookeeper;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.broctagon.trad.networkconfiguration.service.BoCalculationService;
import com.broctagon.trad.networkconfiguration.service.GroupBoSymbolService;
import com.broctagon.trad.networkconfiguration.service.GroupSpreadService;
import com.broctagon.trad.networkconfiguration.service.SecuritySpreadService;
import com.broctagon.trad.networkconfiguration.service.SgIpUrlMapConfigService;
import com.broctagon.trad.networkconfiguration.service.SymbolSpreadConfigService;

/**
* @auther: Water
* @time: Oct 16, 2017 10:41:35 AM
* 
*/

@Component
public class ZookeeperDispatcher {
	
	@Autowired private SgIpUrlMapConfigService sgIpUrlMapConfigService;
	@Autowired private BoCalculationService boCalculationService;
	@Autowired private GroupBoSymbolService groupBoSymbolService;
	@Autowired private GroupSpreadService groupSpreadService;
	@Autowired private SecuritySpreadService securitySpreadService;
	@Autowired private SymbolSpreadConfigService symbolSpreadConfigService;
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	public void dispatch(String path, byte[] data){
		logger.info("NodeDataChanged: event.getPath():" + path);	
		logger.info("NodeDataChanged: data:" + new String(data));
		
		switch(path){
			case ZookeeperConstants.ZK_PATH_SYMBOLSPREAD : 
			//	symbolSpreadConfigService.updateConfigAndNotify(data);
				break;
			case ZookeeperConstants.ZK_PATH_SGIPURLMAP : 
			//	sgIpUrlMapConfigService.updateConfigAndNotify(data);
				break;
			case ZookeeperConstants.ZK_PATH_SECURITYSPREAD :
				securitySpreadService.updateConfigAndNotify(data);
				break;
			case ZookeeperConstants.ZK_PATH_GROUPSPREAD :
				groupSpreadService.updateConfigAndNotify(data);
				break;
		    case ZookeeperConstants.ZK_PATH_BOCALCULATION : 
		    	boCalculationService.updateConfigAndNotify(data);
		    	break;
		    case ZookeeperConstants.ZK_PATH_GROUPBOSYMBOL :
		    	groupBoSymbolService.updateConfigAndNotify(data);
		    	break;		
			default: logger.info("unknown path");
				break;
		}
		
	}

}
