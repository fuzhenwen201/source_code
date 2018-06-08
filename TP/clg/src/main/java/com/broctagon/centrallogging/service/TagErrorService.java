package com.broctagon.centrallogging.service;

import java.lang.management.ManagementFactory;
import java.lang.management.RuntimeMXBean;
import java.math.BigDecimal;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.centrallogging.constant.ErrorMessageType;
import com.broctagon.centrallogging.constant.Tag;
import com.broctagon.centrallogging.dao.TagErrorDao;
import com.broctagon.centrallogging.model.HeartBeat;
import com.broctagon.centrallogging.model.HeartBeatResponse;
import com.broctagon.centrallogging.prop.MQProperties;
import com.broctagon.tradtnode.rabbit.enums.TNodeChannelMsgType;
import com.broctagon.tradtnode.rabbit.message.Message;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;
import com.fasterxml.jackson.databind.ObjectMapper;

@Service
public class TagErrorService {
	
	@Autowired
	private MQProperties mqProperties;

	@Autowired
	private TagErrorDao tagErrorRepository;
	
	public void saveTagError(Message message) {
		tagErrorRepository.saveTagError(message);
	}
	
	public String getTagError(com.broctagon.tradtnode.rabbit.message.Message tnodeMessage) {
		StringBuffer sb = new StringBuffer();
		sb.append(ErrorMessageType.TAG_ERR).append("(responsequeue, tag, messagetype, body)")
		.append(" VALUES('").append(tnodeMessage.getResponceQueue()).append("',")
		.append(tnodeMessage.getTag()).append(",'").append(tnodeMessage.getMessageType()).append("', '")
		.append(tnodeMessage.getMessageString().trim()).append("');");
		return sb.toString();
	}
	
	public byte[] getBody(HeartBeat heartBeat) throws Throwable {
		// get pid
		RuntimeMXBean runtime = ManagementFactory.getRuntimeMXBean();  
        String name = runtime.getName();
		
		HeartBeatResponse response = new HeartBeatResponse();
		response.setReqid(heartBeat.getReqid());
		response.setStatus(3); // running
		response.setName("CLG");
		response.setPid(Integer.parseInt(name.substring(0, name.indexOf('@'))));
		response.setMid(mqProperties.getMid());
		response.setTimestamp(new BigDecimal(System.currentTimeMillis()*1000));
		response.setStarttimes(heartBeat.getTimestamp());
		
		com.broctagon.tradtnode.rabbit.message.Message message = new com.broctagon.tradtnode.rabbit.message.Message(); 
		message.setResponceQueue("MONITORANS." + heartBeat.getMid());
		message.setMessageType(TNodeChannelMsgType.TNODE_GROUP_MSG_TYPE);
		message.setTag(Tag.MONITOR_TAG_RESPONSE);
		message.setMessageString(new ObjectMapper().writeValueAsString(response));
		
		return MessageHelper.prepareMessage(message);
	}
}
