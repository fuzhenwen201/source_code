
package com.broctagon.trad.networkconfiguration.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import com.broctagon.trad.networkconfiguration.zookeeper.ZookeeperConstants;

/**
* @auther: Water
* @time: Oct 27, 2017 1:07:25 PM
* 
*/

@Controller
public class IndexController {

	 @RequestMapping(UrlConstants.URL_NC_INDEX)
	 @ResponseBody
	 String home() {
	    return  
	    		"nc-follower info" 
	    		
	    		+ UrlConstants.HTML_CHANGELINE + UrlConstants.HTML_CHANGELINE + "db:"
	    		
	    		+ UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_DB_SECURITY_BO
	    		+ UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_DB_SECURITY_FX
	    		+ UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_DB_GROUP_BO
	    		+ UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_DB_GROUP_FX
	    
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.HTML_CHANGELINE + "zookeeper path:"
	            
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_ZK_PRE + ZookeeperConstants.ZK_PATH_SYMBOLSPREAD
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_ZK_PRE + ZookeeperConstants.ZK_PATH_SECURITYSPREAD
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_ZK_PRE + ZookeeperConstants.ZK_PATH_GROUPSPREAD
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_ZK_PRE + ZookeeperConstants.ZK_PATH_BOCALCULATION
	            + UrlConstants.HTML_CHANGELINE + UrlConstants.URL_NC_ZK_PRE + ZookeeperConstants.ZK_PATH_GROUPBOSYMBOL
	            ;
	            
	 }
	 	
}
