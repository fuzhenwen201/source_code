package com.broctagon.tradstnode.rabbit.message;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.apache.log4j.Logger;

import com.broctagon.tradstnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradstnode.rabbit.enums.TNodeCnEventHeader;
import com.broctagon.tradstnode.rabbit.enums.TNodeCnServiceHeader;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.broctagon.tradstnode.rabbit.message.MessageHelper;

public class MessageHelper {
	
	private static Logger LOGGER = Logger.getLogger(MessageHelper.class);
	
	private static int totallen = 0;
	
	public static byte[] prepareMessageForCentralDataBase(Message message) throws Throwable {
		String messageString = message.messageString;
		TNodeChannelMsgType messageType = message.messageType;
		short tag = message.tag;
		byte[] messageBytes = messageString.getBytes();
		if (messageType == TNodeChannelMsgType.TNODE_SERVICE_MSG_TYPE) {
			TNodeCnServiceHeader serviceHeader = new TNodeCnServiceHeader();
			serviceHeader.flag = (byte) (0x8f);
			serviceHeader.msg_type = (byte) 1;
			serviceHeader.msg_len = messageBytes.length + 1;
			serviceHeader.tag = tag;
			byte[] sheader = serviceHeader.getBytes();
			int headerlength = sheader.length;
			int messageLength = messageBytes.length;
			int finallength = headerlength + messageLength  + 2;
			byte result[] = new byte[finallength];
			for (int i = 0; i < headerlength; i++) {
				result[i] = sheader[i];
			}
			for (int i = headerlength; i < (headerlength + messageLength); i++) {
				result[i] = messageBytes[i - headerlength];
			}
			@SuppressWarnings("unused")
			int hmlength = headerlength + messageLength;

			return result;
		} else {
			
			TNodeCnEventHeader eventHeader = new TNodeCnEventHeader();
			eventHeader.flag = (byte) (0x8f);
			if (messageType == TNodeChannelMsgType.TNODE_EVENT_MSG_TYPE) {
				eventHeader.msg_type = (byte) (0);
			} else if (messageType == TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE) {
				eventHeader.msg_type = (byte) (2);
			}
			eventHeader.msg_len = messageBytes.length + 1;
			eventHeader.tag = tag;
			byte[] data = eventHeader.getBytes(isLittleIndian());
			int headerlength = data.length;
			LOGGER.info("Header length:-" + headerlength);
			int finallength = headerlength + messageBytes.length + 1;
			byte[] rbyte = new byte[finallength];
			for (int i = 0; i < headerlength; i++) {
				rbyte[i] = data[i];
			}
			for (int i = 0; i < messageBytes.length; i++) {
				rbyte[headerlength + i] = messageBytes[i];
			}
			return rbyte;
		}
	}
	
	public static ByteBuffer prepareMessageForCentralDataBaseSBE(ByteBuffer byteBuffer, short msgTag, int sbelen)
			throws Throwable {
		short tag = msgTag;
		byte[] messageBytes = byteBuffer.array();
		TNodeCnEventHeader eventHeader = new TNodeCnEventHeader();
		eventHeader.flag = (byte) (0x8f);
		eventHeader.msg_type = (byte) (0);
		eventHeader.tag = tag;
		eventHeader.msg_len = sbelen + 1;
		eventHeader.pad = 0;
		totallen = totallen + sbelen + 12 + 1;
		byte[] data = eventHeader.getBytes(isLittleIndian());
		int headerlength = data.length;
		int finallength = headerlength + sbelen + 1;
		byte[] rbyte = new byte[finallength];
		for (int i = 0; i < headerlength; i++) {
			rbyte[i] = data[i];
		}
		for (int i = 0; i < sbelen; i++) {
			rbyte[headerlength + i] = messageBytes[i];
		}
		@SuppressWarnings("unused")
		ByteBuffer sbemessage = ByteBuffer.allocate(rbyte.length);
		return ByteBuffer.wrap(rbyte);

	}
	
	public static byte[] prepareMessage(Message message) throws Throwable {
		String messageString = message.messageString;
		LOGGER.info("Recevie messageString length = " +  messageString.length());
		TNodeChannelMsgType messageType = message.messageType;
		short tag = message.tag;
		//String responseQueueName = message.responceQueue;

		/*if (messageType == TNodeChannelMsgType.TNODE_SERVICE_MSG_TYPE && responseQueueName == "") {
			throw new Exception("For service message, please specify response queue name.");
		}*/
		byte[] messageBytes = messageString.getBytes();
		if (messageType == TNodeChannelMsgType.TNODE_SERVICE_MSG_TYPE) {
			TNodeCnServiceHeader serviceHeader = new TNodeCnServiceHeader();
			serviceHeader.flag = (byte) (0x8f);
			serviceHeader.msg_type = (byte) 1;
			serviceHeader.msg_len = messageBytes.length + 1;
			LOGGER.info("Recevie messageBytes length = " +  messageBytes.length + " serviceHeader.msg_len = "+ serviceHeader.msg_len );
			/*if (responseQueueName == null) {
				LOGGER.info("response q null");
			} else {
				serviceHeader.queue_name_len = (int) responseQueueName.length() + 1;
			}*/
			serviceHeader.tag = tag;
			//byte[] queueNameBytes = responseQueueName.getBytes();
			byte[] sheader = serviceHeader.getBytes();
			int headerlength = sheader.length;
			int messageLength = messageBytes.length;
			LOGGER.info("Recevie headerlength = " +  headerlength + " messageLength = "+ messageLength );
			//int queueNameLength = queueNameBytes.length;
			int finallength = headerlength + messageLength + 1;
			byte result[] = new byte[finallength];
			for (int i = 0; i < headerlength; i++) {
				result[i] = sheader[i];
			}
			for (int i = headerlength; i < (headerlength + messageLength); i++) {
				result[i] = messageBytes[i - headerlength];
			}

			//int hmlength = headerlength + messageLength;
			/*for (int i = 0; i < queueNameLength; i++) {
				result[hmlength + i] = queueNameBytes[i];
			}*/

			return result;
		} else {
			TNodeCnEventHeader eventHeader = new TNodeCnEventHeader();
			eventHeader.flag = (byte) (0x8f);
			if (messageType == TNodeChannelMsgType.TNODE_EVENT_MSG_TYPE) {
				eventHeader.msg_type = (byte) (0);
			} else if (messageType == TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE) {
				eventHeader.msg_type = (byte) (2);
			}
			eventHeader.msg_len = messageBytes.length + 1;
			eventHeader.tag = tag;
			byte[] data = eventHeader.getBytes(isLittleIndian());
			int headerlength = data.length;
			LOGGER.info("Header length:-" + headerlength);
			int finallength = headerlength + messageBytes.length + 1;
			byte[] rbyte = new byte[finallength];
			for (int i = 0; i < headerlength; i++) {
				rbyte[i] = data[i];
			}
			for (int i = 0; i < messageBytes.length; i++) {
				rbyte[headerlength + i] = messageBytes[i];
			}
			return rbyte;

		}
	}

	public static Message parseMessage(byte[] incoming) {
		LOGGER.info("incoming length = "+ incoming.length);
		Message parsedMessage = new Message();
		try {
			byte[] MessagePart = new byte[incoming.length - 12];
			for (int i = 12; i < incoming.length - 1; i++) {
				MessagePart[i - 12] = incoming[i];
			}
			String messageText = new String(MessagePart);
			LOGGER.info("messageText = "+ messageText);
			byte[] headerBytes = new byte[12];
			for (int i = 0; i < 12; i++) {
				headerBytes[i] = incoming[i];
			}
			TNodeCnEventHeader header = TNodeCnEventHeader.deserialize(headerBytes);
			int messageLength = (int) header.msg_len;
			LOGGER.info("MessageLength = " + messageLength + " header.msg_type =" + header.msg_type + " header.tag = " + header.tag);
			/*if (header.msg_type == 1) {
				//messageLength = (int) header.pad;
				LOGGER.info("Messagelen: = " + messageLength);
				parsedMessage.responceQueue = messageText.substring(messageLength, messageText.length() - 1);
			}*/
			//parsedMessage.messageString = messageText.substring(0, messageLength-1);
			parsedMessage.messageString = messageText;
			parsedMessage.tag = header.tag;
			parsedMessage.messageType = getHeaderInt(header.msg_type);

		} catch (Exception ex) {
			LOGGER.error("Error Followed By : " , ex);
			ex.printStackTrace();
		}

		return parsedMessage;
	}
	
	public static Message parseMessage2(byte[] incoming) {
		Message parsedMessage = new Message();
		try {
			byte[] MessagePart = new byte[incoming.length - 8];
			for (int i = 8; i < incoming.length - 1; i++) {
				MessagePart[i - 8] = incoming[i];
			}
			String messageText = new String(MessagePart);
			LOGGER.info("messageText = "+ messageText);
			byte[] headerBytes = new byte[8];
			for (int i = 0; i < 8; i++) {
				headerBytes[i] = incoming[i];
			}
			TNodeCnEventHeader header = TNodeCnEventHeader.deserialize(headerBytes);

			int messageLength = (int) header.msg_len;

			LOGGER.info("MessageLength = " + messageLength + " header.msg_type =" + header.msg_type + " header.tag = " + header.tag);
			/*if (header.msg_type == 1) {
				messageLength = (int) header.pad;
				LOGGER.info("Messagelen: = " + messageLength);
				parsedMessage.responceQueue = messageText.substring(messageLength, messageText.length() - 1);
			}*/
			parsedMessage.messageString = messageText.substring(0, messageLength-1);
			parsedMessage.tag = header.tag;
			parsedMessage.messageType = getHeaderInt(header.msg_type);

		} catch (Exception ex) {
			LOGGER.error("Error Followed By : " , ex);
			ex.printStackTrace();
		}

		return parsedMessage;
	}
	
	private static TNodeChannelMsgType getHeaderInt(int type) {
		switch (type) {
		case 0:
			return TNodeChannelMsgType.TNODE_EVENT_MSG_TYPE;
		case 1:
			return TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE;
		case 3:
			return TNodeChannelMsgType.TNODE_SERVICE_MSG_TYPE;
		default:
			return null;
		}
	}

	public static boolean isLittleIndian() {
		if (ByteOrder.nativeOrder().equals(ByteOrder.BIG_ENDIAN)) {
			return false;
		} else {
			return true;
		}
	}
}
