package cn.fintechstar.admin.listener;

import org.apache.log4j.Logger;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageProperties;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.google.gson.Gson;

import cn.fintechstar.admin.constant.ResponseTitle;
import cn.fintechstar.admin.constant.Tag;
import cn.fintechstar.admin.model.security.message.BasicSecurityAns;
import cn.fintechstar.admin.model.security.message.GetSecuritiesAns;
import cn.fintechstar.admin.service.SecurityService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class SecurityListener {
	
	private static final Logger LOGGER = Logger.getLogger(SecurityListener.class);

	@Autowired
	private SecurityService securityService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	@RabbitListener(queues="SECURITY.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.CREATE_SECURITY_REQ:
			BasicSecurityAns createSecurityAns = securityService.createOrUpdateSecurity(tNode.getBody());
			TNode createSecurityAnsTNode = TNodeCodec.getTNode(Tag.CREATE_SECURITY_ANS, new Gson().toJson(createSecurityAns));
			rabbitTemplate.send(ResponseTitle.SECURITY_TITLE + createSecurityAns.getSgId(), new Message(TNodeCodec.encodeMessage(createSecurityAnsTNode), new MessageProperties()));
			break;
		case Tag.UPDATE_SECURITY_REQ:
			BasicSecurityAns updateSecurityAns = securityService.createOrUpdateSecurity(tNode.getBody());
			TNode updateSecurityAnsTNode = TNodeCodec.getTNode(Tag.UPDATE_SECURITY_ANS, new Gson().toJson(updateSecurityAns));
			rabbitTemplate.send(ResponseTitle.SECURITY_TITLE + updateSecurityAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateSecurityAnsTNode), new MessageProperties()));
			break;
		case Tag.GET_SECURITIES_REQ:
			GetSecuritiesAns getSecuritiesAns = securityService.getSecurities(tNode.getBody());
			TNode getSecuritiesAnsTNode = TNodeCodec.getTNode(Tag.GET_SECURITIES_ANS, new Gson().toJson(getSecuritiesAns));
			rabbitTemplate.send(ResponseTitle.SECURITY_TITLE + getSecuritiesAns.getSgId(), new Message(TNodeCodec.encodeMessage(getSecuritiesAnsTNode), new MessageProperties()));
			break;
		case Tag.DELETE_SECURITY_BY_ID_REQ:
			BasicSecurityAns deleteSecurityAns = securityService.deleteSecurityById(tNode.getBody());
			TNode deleteSecurityAnsTNode = TNodeCodec.getTNode(Tag.GET_SECURITIES_ANS, new Gson().toJson(deleteSecurityAns));
			rabbitTemplate.send(ResponseTitle.SECURITY_TITLE + deleteSecurityAns.getSgId(), new Message(TNodeCodec.encodeMessage(deleteSecurityAnsTNode), new MessageProperties()));
			break;
		default:
			LOGGER.error("unknown tag from SECURITY.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
}
