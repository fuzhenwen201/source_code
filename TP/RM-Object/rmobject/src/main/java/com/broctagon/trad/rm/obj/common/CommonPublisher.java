/**
 * 
 */
package com.broctagon.trad.rm.obj.common;

import java.util.Date;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.AmqpTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.obj.model.ClosedTradDaoAdminModel;
import com.broctagon.trad.rm.obj.res.message.TraderHistoryResMsg;
import com.broctagon.trad.rm.obj.res.message.TraderPositionRes;
import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;
import com.broctagon.tradstnode.rabbit.publish.Publisher;
import com.broctagon.tradstnode.rabbit.utils.ConstantUtils;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;
import com.broctagon.tradstnode.rabbit.utils.JsonParserUtil;

/**
 * @author imdadullah
 *
 */
@Service
public class CommonPublisher {
	private Logger LOGGER = Logger.getLogger(this.getClass());
	
	@Autowired
	private AmqpTemplate amqpTemplate;

	public void publishCustomizedBoRes(String customBoAns, int sgId) {
		String queName = CommonConstants.GET_CUSTOMBO_ANS + sgId;
		int tag = CommonConstants.GET_CUSTOMBO_ANS_TAG;
		Message ansMessage = new Message();
		ansMessage.setMessageString(customBoAns);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "RES_CUSTOMIZEDBO", tag, customBoAns.length(), customBoAns);
		try {
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<< ======= CUSTOMIZED BO RES AFTER RECEIVING REQ IS ======== >>>>" + customBoAns);
			Publisher.basicPublishByGivenExchangeAndmqTemplate(ConstantUtils.EXCHANGE_DECLARATION_NAME, queName,
					ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
			e.printStackTrace();
		}
	}

	public void publishCustomizedUpdatedBo(String customUpdateStr) {
		String queName = CommonConstants.GET_CUSTOMBO_UPDATE_ANS;
		int tag = CommonConstants.GET_CUSTOMBO_UPDATE_ANS_TAG;
		Message ansMessage = new Message();
		ansMessage.setMessageString(customUpdateStr);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "UPDATED_CUSTOMIZEDBO", tag, customUpdateStr.length(), customUpdateStr);
		try {
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<======== UPDATED CUSTOMIZED BO AFTER TRADE UPDATE IS ======== >>>>" + customUpdateStr);
			Publisher.basicPublishAndmqTemplate(queName, ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
			e.printStackTrace();
		}
	}

	public void publishTop5UpdatedBo(String top5UpdatedStr) {
		int tag = CommonConstants.GET_TOP5BO_UPDATE_ANS_TAG;
		String queName = CommonConstants.GET_TOP5BO_UPDATE_ANS;
		Message ansMessage = new Message();
		ansMessage.setMessageString(top5UpdatedStr);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "UPDATED_TOP5BO", tag, top5UpdatedStr.length(), top5UpdatedStr);
		byte[] ansByte;
		try {
			ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<======== UPDATED TOP FIVE BO AFTER TRADE UPDATE IS  ========>> " + top5UpdatedStr);
			Publisher.basicPublishAndmqTemplate(queName, ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}

	}

	public void publishTop5BoRes(String top5UpdatedStr, int sgId) {

		int tag = CommonConstants.GET_TOP5BO_ANS_TAG;
		String queName = CommonConstants.GET_TOP5BO_ANS + sgId;
		Message ansMessage = new Message();
		ansMessage.setMessageString(top5UpdatedStr);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "RES_TOP5BO", tag, top5UpdatedStr.length(), top5UpdatedStr);
		byte[] ansByte;
		try {
			ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<========TOP FIVE BO RES AFTER RECEIVING REQ IS ========>> " + top5UpdatedStr);
			Publisher.basicPublishAndmqTemplate(queName, ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}
	}

	public void publishSymbolUpdatedBo(String symbolBoStr) {
		String queName = CommonConstants.GET_SYMBOLBO_UPDATE_ANS;
		int tag = CommonConstants.GET_SYMBOLBO_UPDATE_ANS_TAG;
		Message ansMessage = new Message();
		ansMessage.setMessageString(symbolBoStr);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "UPDATED_SYMBOLBO ", tag, symbolBoStr.length(), symbolBoStr);
		try {
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<<======== UPDATED SYMBOL BO AFTER TRADE UPDATE IS ====== >>>>>>" + symbolBoStr);
			Publisher.basicPublishAndmqTemplate(queName, ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
			e.printStackTrace();
		}
	}

	public void publishSymbolBoRes(String symbolBoStr, int sgId) {
		String queName = CommonConstants.GET_SYMBOLBO_ANS + sgId;
		int tag = CommonConstants.GET_SYMBOLBO_ANS_TAG;
		Message ansMessage = new Message();
		ansMessage.setMessageString(symbolBoStr);
		ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		ansMessage.setTag((short) tag);
		RMLogger.writeLogMessage(ConstantUtils.MESSAGE_OUT, "RES_SYMBOLBO ", tag, symbolBoStr.length(), symbolBoStr);
		try {
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<<======== SYMBOL BO RES AFTER RECEIVING REQ IS ====== >>>>>>" + symbolBoStr);
			Publisher.basicPublishAndmqTemplate(queName, ansByte, amqpTemplate);
		} catch (Throwable e) {
			LOGGER.error("ERROR MESSAGE FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	public void publishPositionToSG(TraderPositionRes positionRes, int sgId) {
		String queName = CommonConstants.GET_POSITION_ANS + sgId;
		int tag = CommonConstants.GET_POSITION_ANS_TAG;
		try {
			String positionResMsg = JSON.toJSONString(positionRes);
			LOGGER.info("********* QUE NAME FOR TRADER POSITION IS **** " + queName + "****");
			Message ansMessage = new Message();
			ansMessage.setMessageString(positionResMsg);
			ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
			ansMessage.setTag((short) tag);
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<<<<========POSITION SEND TO SG FOR CMT  =========>>>>>>>>>>" + positionResMsg);
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_OUT, tag, positionResMsg.length(), positionResMsg);
			Publisher.basicPublishByGivenExchangeAndmqTemplate(ConstantUtils.EXCHANGE_DECLARATION_NAME, queName,
					ansByte, amqpTemplate);
			LOGGER.info("POSITION SENT TO SG FOR CMT");
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	public void publishHistoryToSG(TraderHistoryResMsg historyRes, int sgId) {
		String queName = CommonConstants.SENDTRADERPOSITION + sgId;
		int tag = CommonConstants.TRADER_HISTORY_TAG;
		try {
			String historyResMsg = JSON.toJSONString(historyRes);
			LOGGER.info("*******QUE NAME FOR TRADER HISTORY IS **** " + queName + "****");
			Message ansMessage = new Message();
			ansMessage.setMessageString(historyResMsg);
			ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
			ansMessage.setTag((short) tag);
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<<<<======== HISTORY SIZE SEND TO SG FOR TT  =========>>>>>>>>>" + historyResMsg.length());
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_OUT, tag, historyResMsg.length(), "");
			Publisher.basicPublishByGivenExchangeAndmqTemplate(ConstantUtils.EXCHANGE_DECLARATION_NAME, queName,
					ansByte, amqpTemplate);
			LOGGER.info("HISTORY SENT TO SG FOR TT");
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	/**
	 * @param queueName
	 * @param tag
	 * @param updateTradeMsg
	 */
	public void pubUpdatedPositionToMGSG(String queueName, int tag, String updateTradeMsg) {
		try {
			LOGGER.info("******** ROUTING KEY NAME FOR UPDATED TRADE **** " + queueName + "****");
			Message ansMessage = new Message();
			ansMessage.setMessageString(updateTradeMsg);
			ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
			ansMessage.setTag((short) tag);
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("<<<<<<<<<<======== UPDATE POSITION SEND TO SG FOR CMT =========>>>>>>>>>" + updateTradeMsg);
			Publisher.basicPublishByGivenExchangeAndmqTemplate(ConstantUtils.EXCHANGE_DECLARATION_NAME, queueName,
					ansByte, amqpTemplate);
			LOGGER.info("UPDATE POSITION SENT TO MGSG FOR CMT");
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}
	}

	public void publishClosePositionToAdmin(ClosedTradDaoAdminModel tradeListModel) {
		String queName = CommonConstants.PUB_CLOSEPOSITION_ADT;
		int tag = CommonConstants.PUB_CLOSEPOSITION_ADT_TAG;
		try {
			String ansString = JSON.toJSONString(tradeListModel);
			LOGGER.info("QUE NAME FOR TRADEUPDATE LIST FOR ADMIN **** " + queName + "****");
			Message ansMessage = new Message();
			ansMessage.setMessageString(ansString);
			ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
			ansMessage.setTag((short) tag);
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info("MESSAGE SEND TO ADMIN  =" + ansString);
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_OUT, tag, ansString.length(), ansString);
			Publisher.basicPublishByGivenExchange(ConstantUtils.EXCHANGE_DECLARATION_NAME, queName, ansByte,
					amqpTemplate);
			LOGGER.info("CLOSED POSITION SENT TO SG FOR ADMIN");
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		} catch (Throwable e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}
	}

	public void publishEchoMsgToSG(int mid, Object obj, Boolean isTest) {
		Message ansMessage = null;
		try {
			String queueName = CommonConstants.PUB_ECHO_MSG_QUE + mid;
			int tag = CommonConstants.PUB_ECHO_MSG_TAG;
			LOGGER.info(" <<<<< QUE NAME FOR ECHO MSG =====>>>>>" + queueName);
			String ansString = JsonParserUtil.toJsonString(obj);
			ansMessage = new Message();
			ansMessage.setMessageString(ansString);
			ansMessage.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
			ansMessage.setTag((short) tag);
			byte[] ansByte = MessageHelper.prepareMessage(ansMessage);
			LOGGER.info(" <<<<< GOING TO PUBLISH ECHO MSG ====>>>>>>" + ansString);
			RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
					ConstantUtils.MESSAGE_OUT, tag, ansString.length(), ansString);
			Publisher.basicPublishByGivenExchange(ConstantUtils.EXCHANGE_DECLARATION_NAME, queueName, ansByte,
					amqpTemplate);
			LOGGER.info("Message send");
		} catch (Throwable e) {
			LOGGER.error("ERROR MESSAGE :", e);
			e.printStackTrace();
		}
	}

}
