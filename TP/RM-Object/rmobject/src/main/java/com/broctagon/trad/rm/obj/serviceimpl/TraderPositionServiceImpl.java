/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.dao.OpenTradeDao;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.message.UpdateClosedPositionMsg;
import com.broctagon.trad.rm.obj.model.UpdateClosedPositionModel;
import com.broctagon.trad.rm.obj.model.UpdateOpenPositionModel;
import com.broctagon.trad.rm.obj.req.message.TraderPositionReq;
import com.broctagon.trad.rm.obj.res.message.TraderPositionRes;
import com.broctagon.trad.rm.obj.service.TraderPositionService;
import com.broctagon.tradstnode.rabbit.message.Message;



/**
 * @author imdadullah
 *
 */
@Service
public class TraderPositionServiceImpl implements TraderPositionService {
	Logger LOGGER = Logger.getLogger(TraderPositionServiceImpl.class);
	@Autowired
	CommonPublisher commonPublisher;
	@Autowired
	OpenTradeDao openTradedao;

	public void tradersPosition(Message message, int reqTag) {
		List<TradeModel> tradeList;
		String messageString = message.getMessageString();
		TraderPositionReq req = JSON.parseObject(messageString, TraderPositionReq.class);
		LOGGER.info(">>>>>> GOING TO SEND POSITIONS  <<<<<<<");
		LOGGER.info("REQUEST SG ID IS >>>>>>>>>>>>>" + req.getSgid());
		tradeList = getOpenTradeList(req.getClientid());
		int size = tradeList.size();
		if (size > 20) {
			int loopNo = (size / 20);
			for (int i = 0; i < loopNo; i++) {
				try {
					List<TradeModel> tradeSubList = tradeList.subList(0, 20);
					TraderPositionRes positionRes = new TraderPositionRes();
					positionRes.setClientid(req.getClientid());
					positionRes.setManagerid(req.getManagerid());
					positionRes.setReqid(req.getReqid());
					positionRes.setBody(tradeSubList);
					//positionRes.setTotal(tradeSubList.size());
					positionRes.setTotal(size);
					positionRes.setSgid(req.getSgid());
					positionRes.setSession(req.getSession());
					LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS POSITION >>>>>>>>>>");
					commonPublisher.publishPositionToSG(positionRes, req.getSgid());
					tradeSubList.subList(0, 20).clear();
				} catch (Exception e) {
					LOGGER.error("ERROR FOLLOWED BY  : ", e);
					e.printStackTrace();
				}
			}

			TraderPositionRes positionRes = new TraderPositionRes();
			positionRes.setClientid(req.getClientid());
			positionRes.setManagerid(req.getManagerid());
			positionRes.setReqid(req.getReqid());
			positionRes.setBody(tradeList);
			positionRes.setTotal(tradeList.size());
			positionRes.setSgid(req.getSgid());
			positionRes.setSession(req.getSession());
			LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS POSITION >>>>>>>>>>");
			commonPublisher.publishPositionToSG(positionRes, req.getSgid());

		} else {
		//	if (size != 0) {
				TraderPositionRes positionRes = new TraderPositionRes();
				positionRes.setClientid(req.getClientid());
				positionRes.setManagerid(req.getManagerid());
				positionRes.setReqid(req.getReqid());
				positionRes.setBody(tradeList);
				positionRes.setTotal(tradeList.size());
				positionRes.setSgid(req.getSgid());
				positionRes.setSession(req.getSession());
				System.out.println("Position dadat is " +positionRes);
				LOGGER.info(" <<<<<<<<<<<<<<< GOING TO SEND TRADERS POSITION >>>>>>>>>>");
				commonPublisher.publishPositionToSG(positionRes, req.getSgid());
		//	}

		}
	}

	public List<TradeModel> getOpenTradeList(int clientId) {
		List<TradeModel> tradeList = null;
		try {
			if (clientId > 0) {
				tradeList = openTradedao.selectOpenTradeByClientid(clientId);
			} else if (clientId == 0) {
				tradeList = openTradedao.selectAllOpenTrade();
			}
			return tradeList;
		} catch (Exception e) {
			LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
			e.printStackTrace();
			return null;
		}
	}

	public void updateTraderOpenPosition(List<TradeModel> openTradeList) {

		/*List<TradeModel> openTradeList = Stream.concat(openUpListForUpdate.stream(), openDownListForUpdate.stream())
				.collect(Collectors.toList());*/

		if (openTradeList.size() > 0) {
			UpdateOpenPositionModel updateAns = new UpdateOpenPositionModel();
			String routingKeyName = CommonConstants.GET_UPDATEOPENTRADE_RES;
			int tag = CommonConstants.GET_UPDATEOPENTRADE_TAG;
			updateAns.setBody(openTradeList);
			String ansMsg = JSON.toJSONString(updateAns);
			commonPublisher.pubUpdatedPositionToMGSG(routingKeyName, tag, ansMsg);
		}
	}

	public void updateTraderClosedPosition(List<TradeModel> closeTradeList) {
		if (closeTradeList == null)
			return;
		String routingKeyName = CommonConstants.GET_UPDATECLOSEDTRADE_RES;
		int tag = CommonConstants.GET_UPDATECLOSEDTRADE_TAG;
		UpdateClosedPositionModel updateAns = new UpdateClosedPositionModel();
		List<UpdateClosedPositionMsg> msgList = new ArrayList<UpdateClosedPositionMsg>();
		Map<String, List<TradeModel>> closedPositionMap = closeTradeList.stream()
				.collect(Collectors.groupingBy(o -> o.getOrderid()));
		List<String> orderIdList = closedPositionMap.keySet().stream().collect(Collectors.toList());
		for (String orderId : orderIdList) {
			UpdateClosedPositionMsg closePositionMsg = new UpdateClosedPositionMsg();
			closePositionMsg.setOrderid(orderId);
			msgList.add(closePositionMsg);
		}
		updateAns.setBody(msgList);
		String ansMsg = JSON.toJSONString(updateAns);
		commonPublisher.pubUpdatedPositionToMGSG(routingKeyName, tag, ansMsg);
	}
}
