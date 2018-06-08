/**
 * 
 */
package com.broctagon.trad.rm.obj.req.serviceimpl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.req.message.CustomBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.SymbolBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.TopFiveBoReqMsg;
import com.broctagon.trad.rm.obj.service.BoReqService;
import com.broctagon.trad.rm.obj.service.BoResService;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.google.gson.Gson;


/**
 * @author imdadullah
 *
 */
@Service
@Scope(value = "singleton")
public class BoReqServiceImpl implements BoReqService {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	private Map<Integer, TopFiveBoReqMsg> topBoReqMap = new HashMap<>();
	private Map<Integer, List<CustomBoReqMsg>> customReqMap = new HashMap<>();
	private Map<Integer, SymbolBoReqMsg> symbolReqMap = new HashMap<>();

	@Autowired
	private BoResService boResService;
	@Autowired
	private CommonPublisher commonPublisher;

	public Map<Integer, TopFiveBoReqMsg> getTopBoReqMap() {
		return topBoReqMap;
	}

	public Map<Integer, List<CustomBoReqMsg>> getCustomReqMap() {
		return customReqMap;
	}

	public Map<Integer, SymbolBoReqMsg> getSymbolReqMap() {
		return symbolReqMap;
	}

	public void addTopFive5BoReq(String boReqMsgStr) {
		TopFiveBoReqMsg req = null;
		String topBoResStr = null;
		req = JSON.parseObject(boReqMsgStr, TopFiveBoReqMsg.class);
		int managerId = req.getManagerid();
		int direction = req.getDirection();
		LOGGER.info("<<<<<<<< MANAGER ID IS =====>>>> [" + managerId + "] <<<<<=====");
		try {
			topBoReqMap.put(managerId, req);
			LOGGER.info("<<<<<<<<< TOP5BO REQ LIST SIZE IS =====>>>>>[" + topBoReqMap.size() + "] <<<<<=====");
			if (direction == 1) {
				topBoResStr = boResService.getTop5BoUpRes(req);
			} else {
				topBoResStr = boResService.getTop5BoDownRes(req);
			}
			if (topBoResStr != null) {
				LOGGER.info("@@@@@@@@ GOING TO PUB TOP5BO AFTER GETTING REQ FOR DIR@@@@@@@@@ [" + direction + "]");
				commonPublisher.publishTop5BoRes(topBoResStr, req.getSgid());
			} else {
				LOGGER.info("######## NO TOP5BO TO PUB AFTER GETTING REQ ########[" + direction + "]");
			}

		} catch (Exception e) {
			LOGGER.error("ERROR MESSAGE :", e);
			e.printStackTrace();
		}

	}

	public void unSubTopFive5BoReq(String messageString) {
		TopFiveBoReqMsg req = null;
		req = JSON.parseObject(messageString, TopFiveBoReqMsg.class);
		int managerId = req.getManagerid();
		LOGGER.info("<<<<<<< MANAGER ID IS =====>>>> [" + managerId + "] <<<<<=====");
		try {
			LOGGER.info("<<<<<< TOP5BO REQ SIZE BEFORE UNSUB >>>>>>>" + topBoReqMap.size());
			topBoReqMap.remove(managerId);
			LOGGER.info("<<<<<< TOP5BO REQ SIZE AFTER UNSUB >>>>>>>" + topBoReqMap.size());
		} catch (Exception e) {
			LOGGER.error("ERROR MESSAGE", e);
			e.printStackTrace();
		}
	}

	public void addCustomBoReq(String customReqMsgStr) {
		Gson gson = new Gson();
		CustomBoReqMsg reqest = null;
		LOGGER.info("<<<<<<< CUSTOMIZED REQ MSG IS =====>>>> " + customReqMsgStr);
		reqest = gson.fromJson(customReqMsgStr.trim(), CustomBoReqMsg.class);
		int asseteId = reqest.getAssetid();
		LOGGER.info("CUSTOMIZED ASSET ID IS ====>>>>> [ " + asseteId + " ] <<<<<=======");
		try {
			if (customReqMap.containsKey(asseteId)) {
				List<CustomBoReqMsg> customBoReqList = customReqMap.get(asseteId);
				if (!customBoReqList.contains(reqest)) {
					customBoReqList.add(reqest);
					customReqMap.put(asseteId, customBoReqList);
				}
			} else {
				List<CustomBoReqMsg> customBoReqList = new ArrayList<>();
				customBoReqList.add(reqest);
				customReqMap.put(asseteId, customBoReqList);
			}
			LOGGER.info(
					"<<<<<< SIZE OF CUSTOMIZED REQUEST LIST IS  ======>>> [ " + customReqMap.size() + " ] <<<========");
			String boResStr = boResService.getCustomBoRes(reqest);
			if (boResStr != null) {
				commonPublisher.publishCustomizedBoRes(boResStr, reqest.getSgid());
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
	}

	public void unSubCustomBoReq(String customReqMsgStr) {
		Gson gson = new Gson();
		CustomBoReqMsg customReq = null;
		customReq = JSON.parseObject(customReqMsgStr, CustomBoReqMsg.class);
		int assetId = customReq.getAssetid();
		int managerId = customReq.getManagerid();
		int direction = customReq.getDirection();
		System.out.println("Received unsub request" + customReqMsgStr);
		try {
			List<CustomBoReqMsg> customBoReqList = customReqMap.get(assetId);
			LOGGER.info("<<<<<< CUSTOM REQ SIZE BEFORE UNSUB >>>>>>>" + customBoReqList.size());
			customBoReqList.remove(customReq);
			LOGGER.info("########## CUSTOM REQ SIZE AFTER UNSUB #######" + customBoReqList.size());
			if (customBoReqList.size() > 0) {
				customReqMap.put(assetId, customBoReqList);
			} else {
				customReqMap.remove(assetId);
			}
			LOGGER.info("########## MANAGER REQ SIZE FOR CUSTOM AFTER UNSUB #######" + customReqMap.size());
			CustomBoReqMsg removeReq = new CustomBoReqMsg();
			removeReq.setAssetid(assetId);
			removeReq.setManagerid(managerId);
			removeReq.setReqid(customReq.getReqid());
			removeReq.setSession(customReq.getSession());
			removeReq.setDirection(direction);
			removeReq.setStop(true);
			String removeRes = gson.toJson(removeReq);
			commonPublisher.publishCustomizedBoRes(removeRes, customReq.getSgid());
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
	}

	/**
	 * This method add the request for the Symbol Bo. Send All symbol Bo when
	 * request received.
	 * 
	 * @param message
	 *            is the Request for the SymbolBo
	 */
	public void addSymbolBoReq(Message message) {
		SymbolBoReqMsg req = null;
		req = JSON.parseObject(message.getMessageString().trim(), SymbolBoReqMsg.class);
		int managerId = req.getManagerid();
		try {
			symbolReqMap.put(managerId, req);
			LOGGER.info("<<<<<< SIZE OF SYMBOL BO REQ LIST =====>>>> [ " + symbolReqMap.size() + " ] <<<<=====");
			String boResStr = boResService.getSymbolBoRes(req);
			if (boResStr != null) {
				commonPublisher.publishSymbolBoRes(boResStr, req.getSgid());
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}

	}

	public void unSubSymbolBoReq(Message message) {
		SymbolBoReqMsg req = null;
		req = JSON.parseObject(message.getMessageString().trim(), SymbolBoReqMsg.class);
		int managerId = req.getManagerid();
		try {
			symbolReqMap.remove(managerId);
			LOGGER.info("###### SIZE OF SYMBOL BO REQ LIST AFTER UNSUB REQ =====>>>> [ " + symbolReqMap.size()
					+ " ] <<<<=====");
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
	}

}
