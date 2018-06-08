
package com.broctagon.trad.rm.obj.test;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.model.BalanceTrader;
import com.broctagon.trad.rm.obj.rs.service.UpdateTraderService;

/**
* @auther: Water
* @time: Oct 13, 2017 9:22:38 PM
* 
*/

@Component
public class OnlineUserTest {

	@Autowired
	private UpdateTraderService updateTraderService;
	
	public void getOnlineUserTest(){				
		List<BalanceTrader> balanceTraders = updateTraderService.fetchOnlineBalanceTrader();
		String balanceTradersStr = JSON.toJSONString(balanceTraders);		
		System.out.println("balanceTradersStr:" + balanceTradersStr);		
	}
	
}
