/**
 * 
 */
package com.broctagon.trad.rm.api.controller;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import com.broctagon.trad.rm.api.model.CandleDataRes;
import com.broctagon.trad.rm.api.service.CandleDataService;
import com.broctagon.trad.rm.api.service.HistoryDataService;
import com.broctagon.trad.rm.obj.res.message.TraderHistoryResMsg;
import com.broctagon.trad.rm.obj.serviceimpl.TraderHistoryServiceImpl;

/**
 * @author imdadullah
 *
 */

@RestController
public class HistoryDataController {

	Logger LOGGER = Logger.getLogger(TraderHistoryServiceImpl.class);
	@Autowired
	HistoryDataService historyData;
	@Autowired
	CandleDataService candleData;

	@RequestMapping(value = "/trader/orderlist", method = RequestMethod.POST)
	public @ResponseBody TraderHistoryResMsg tradersHistory(@RequestBody String reqMsg) {
		TraderHistoryResMsg historyRes = null;
		LOGGER.info("Request RCVD >>>>>>>" + reqMsg);
		historyRes = historyData.tradersHistory(reqMsg);
		LOGGER.info("Response is >>>>>>>"+historyRes);
		return historyRes;
	}
	
	@RequestMapping(value = "/trader/historicalData", method = RequestMethod.POST)
	public @ResponseBody CandleDataRes candleData(@RequestBody String reqMsg) {
		CandleDataRes candleDataRes = null;
		LOGGER.info("Request RCVD >>>>>>>" + reqMsg);
		candleDataRes = candleData.getCandleData(reqMsg);
		LOGGER.info("Response is >>>>>>>"+candleDataRes.toString());
		return candleDataRes;
	}
}
