
package com.broctagon.trad.networkconfiguration.controller;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.common.CommonUtil;
import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperConstants;
import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperHelper;

/**
 * @auther: Water
 * @time: Oct 27, 2017 3:25:57 PM
 * 
 */

@Controller
public class ZookeeperController {

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired
	private ZookeeperHelper zookeeperHelper;

	@RequestMapping(value="/zk/{path}", method = RequestMethod.GET)
	@ResponseBody
	public String zkCheck(@PathVariable String path) {

		logger.info("path:" + path);	
		if(CommonUtil.isEmptyString(path)){
			return "path empty";
		}
		
		byte[] content;
		try {	
			path = "/" + path;			
			content = zookeeperHelper.getData(path);
		    return path + "<br/>" + new String(content);
		} catch (Exception e) {
			e.printStackTrace();
			return e.getMessage();
		}

	}

}
