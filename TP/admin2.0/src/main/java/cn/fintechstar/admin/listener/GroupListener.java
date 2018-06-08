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
import cn.fintechstar.admin.model.group.message.BasicGroupAns;
import cn.fintechstar.admin.model.group.message.GetGroupsAns;
import cn.fintechstar.admin.model.group.message.GroupDetailsMessage;
import cn.fintechstar.admin.service.GroupService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class GroupListener {
	
	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(GroupListener.class);

	@Autowired
	private GroupService groupService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	@RabbitListener(queues="GROUP.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.CREATE_GROUP_REQ:
			BasicGroupAns createGroupAns = groupService.createOrUpdateGroup(tNode.getBody());
			TNode createGroupAnsTnode = TNodeCodec.getTNode(Tag.CREATE_GROUP_ANS, GSON.toJson(createGroupAns)); 
			rabbitTemplate.send(ResponseTitle.GROUP_TITLE + createGroupAns.getSgId(), new Message(TNodeCodec.encodeMessage(createGroupAnsTnode), new MessageProperties()));
			break;
		case Tag.UPDATE_GROUP_REQ:
			BasicGroupAns updateGroupAns = groupService.createOrUpdateGroup(tNode.getBody());
			TNode updateGroupAnsTnode = TNodeCodec.getTNode(Tag.CREATE_GROUP_ANS, GSON.toJson(updateGroupAns)); 
			rabbitTemplate.send(ResponseTitle.GROUP_TITLE + updateGroupAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateGroupAnsTnode), new MessageProperties()));
			break;
		case Tag.GET_GROUP_BY_ID_REQ:
			GroupDetailsMessage getGroupByIdAns = groupService.getGroupById(tNode.getBody());
			TNode getGroupIdAnsTnode = TNodeCodec.getTNode(Tag.GET_GROUP_BY_ID_ANS, GSON.toJson(getGroupByIdAns));
			rabbitTemplate.send(ResponseTitle.GROUP_TITLE + getGroupByIdAns.getSgId(), new Message(TNodeCodec.encodeMessage(getGroupIdAnsTnode), new MessageProperties()));
			break;
		case Tag.GET_GROUP_FOR_GATEWAY_REQ:
			// TODO
			break;
		case Tag.GET_GROUPS_REQ:
			GetGroupsAns getGroupsAns = groupService.getGroups(tNode.getBody());
			TNode getGroupsAnsTnode = TNodeCodec.getTNode(Tag.GET_GROUPS_ANS, GSON.toJson(getGroupsAns));
			rabbitTemplate.send(ResponseTitle.GROUP_TITLE + getGroupsAns.getSgId(), new Message(TNodeCodec.encodeMessage(getGroupsAnsTnode), new MessageProperties()));
			break;
		case Tag.DELETE_GROUP_REQ:
			// TODO
			break;
		default:
			LOGGER.error("unknown tag from GROUP.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
}
