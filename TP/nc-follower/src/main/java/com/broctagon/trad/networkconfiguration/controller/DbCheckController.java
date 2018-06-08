
package com.broctagon.trad.networkconfiguration.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.networkconfiguration.db.dao.BoCalculationDao;
import com.broctagon.trad.networkconfiguration.db.dao.GroupBoSymbolsDao;
import com.broctagon.trad.networkconfiguration.db.dao.GroupSpreadDao;
import com.broctagon.trad.networkconfiguration.db.dao.SecuritySpreadDao;
import com.broctagon.trad.networkconfiguration.db.dao.SymbolSpreadDao;

/**
* @auther: Water
* @time: Oct 27, 2017 1:07:38 PM
* 
*/

@Controller
public class DbCheckController {
	
	@Autowired private BoCalculationDao boCalculationDao;
	@Autowired private GroupBoSymbolsDao groupBoSymbolsDao;
	@Autowired private GroupSpreadDao groupSpreadDao;
	@Autowired private SecuritySpreadDao securitySpreadDao;
	@Autowired private SymbolSpreadDao symbolSpreadDao;
	
	 @RequestMapping(UrlConstants.URL_NC_DB_SECURITY_BO)
	 @ResponseBody
	 public String securitybo() {		
	    return "securitybo:" + UrlConstants.HTML_CHANGELINE + JSON.toJSONString(boCalculationDao.queryBoCalculationModels());
	 }
	 
	 @RequestMapping(UrlConstants.URL_NC_DB_SECURITY_FX)
	 @ResponseBody
	 public String securityfx() {
		 return "securityfx:" + UrlConstants.HTML_CHANGELINE + JSON.toJSONString(securitySpreadDao.querySecuritySpreadModels());
	 }
	 
	 @RequestMapping(UrlConstants.URL_NC_DB_GROUP_BO)
	 @ResponseBody
	 public String groupbo() {
		 return "groupbo:" + UrlConstants.HTML_CHANGELINE + JSON.toJSONString(groupBoSymbolsDao.queryGroupBoSymbolsModels());
	 }
	 
	 @RequestMapping(UrlConstants.URL_NC_DB_GROUP_FX)
	 @ResponseBody
	 public String groupfx() {
		 return "groupfx:" + UrlConstants.HTML_CHANGELINE + JSON.toJSONString(groupSpreadDao.queryGroupSpreadModels());
	 }

}
