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
import cn.fintechstar.admin.model.manager.message.AddManagerToGroupAns;
import cn.fintechstar.admin.model.manager.message.BasicManagerAns;
import cn.fintechstar.admin.model.manager.message.DeleteManager;
import cn.fintechstar.admin.model.manager.message.EnableManagerAns;
import cn.fintechstar.admin.model.manager.message.GetManagerAccessRightAns;
import cn.fintechstar.admin.model.manager.message.GetManagersAns;
import cn.fintechstar.admin.service.ManagerService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class ManagerListener {

	private static final Gson GSON = new Gson();

	private static final Logger LOGGER = Logger.getLogger(ManagerListener.class);

	@Autowired
	private RabbitTemplate rabbitTemplate;

	@Autowired
	private ManagerService managerService;
	
	@RabbitListener(queues = "MANAGER.AD")
	public void receiveMessage(Message message) {
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
			case Tag.CREATE_MANAGER_REQ:
				BasicManagerAns createManagerAns = managerService.createOrUpdateManager(tNode.getBody());
				TNode createManagerAnsTnode = TNodeCodec.getTNode(Tag.CREATE_MANAGER_ANS, GSON.toJson(createManagerAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + createManagerAns.getSgId(), new Message(TNodeCodec.encodeMessage(createManagerAnsTnode), new MessageProperties()));
				break;
			case Tag.UPDATE_MANAGER_REQ:
				BasicManagerAns updateManagerAns = managerService.createOrUpdateManager(tNode.getBody());
				TNode updateManagerAnsTnode = TNodeCodec.getTNode(Tag.UPDATE_MANAGER_ANS, GSON.toJson(updateManagerAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + updateManagerAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateManagerAnsTnode), new MessageProperties()));
				break;
			case Tag.UPDATE_MANAGER_ACCESS_REQ:
				BasicManagerAns updateManagerAccessRightAns = managerService.updateManagerAccessRight(tNode.getBody());
				TNode updateManagerAccessRightAnsTnode = TNodeCodec.getTNode(Tag.UPDATE_MANAGER_ACCESS_ANS, GSON.toJson(updateManagerAccessRightAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + updateManagerAccessRightAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateManagerAccessRightAnsTnode), new MessageProperties()));
				break;
			case Tag.DELETE_MANAGER_REQ:
				DeleteManager deleteManagerAns = managerService.deleteManager(tNode.getBody());
				TNode deleteManagerAnsTnode = TNodeCodec.getTNode(Tag.DELETE_MANAGER_ANS, GSON.toJson(deleteManagerAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + deleteManagerAns.getSgId(), new Message(TNodeCodec.encodeMessage(deleteManagerAnsTnode), new MessageProperties()));
				break;
			case Tag.GET_MANAGERS_REQ:
				GetManagersAns getManagersAns = managerService.getManagers(tNode.getBody());
				TNode getManagersAnsTnode = TNodeCodec.getTNode(Tag.GET_MANAGERS_ANS, GSON.toJson(getManagersAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + getManagersAns.getSgId(), new Message(TNodeCodec.encodeMessage(getManagersAnsTnode), new MessageProperties()));
				break;
			case Tag.ENABLE_MANAGER_REQ:
				EnableManagerAns enableManagerAns = managerService.enableManager(tNode.getBody());
				TNode enableManagerAnsTnode = TNodeCodec.getTNode(Tag.ENABLE_MANAGER_ANS, GSON.toJson(enableManagerAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + enableManagerAns.getSgId(), new Message(TNodeCodec.encodeMessage(enableManagerAnsTnode), new MessageProperties()));
				break;
			case Tag.GET_MANAGER_RIGHT_REQ:
				GetManagerAccessRightAns getManagerAccessRightAns = managerService.getManagerAccessRights(tNode.getBody());
				TNode getManagerAccessRightsAnsTnode = TNodeCodec.getTNode(Tag.GET_GROUP_BY_MANAGER_ID_ANS, GSON.toJson(getManagerAccessRightAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + getManagerAccessRightAns.getSgId(), new Message(TNodeCodec.encodeMessage(getManagerAccessRightsAnsTnode), new MessageProperties()));
				break;
			case Tag.ADD_MANAGER_TO_GROUP_REQ:
				AddManagerToGroupAns addManagerToGroupAns = managerService.addManagerToGroup(tNode.getBody());
				TNode addManagerToGroupAnsTnode = TNodeCodec.getTNode(Tag.ADD_MANAGER_TO_GROUP_ANS, GSON.toJson(addManagerToGroupAns));
				rabbitTemplate.send(ResponseTitle.MANAGER_TITLE + addManagerToGroupAns.getSgId(), new Message(TNodeCodec.encodeMessage(addManagerToGroupAnsTnode), new MessageProperties()));
				break;
			case Tag.GET_GROUP_BY_MANAGER_ID_REQ:
				break;
			default:
				LOGGER.error("unknown tag from GROUP.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
				break;
		}
	}
}
