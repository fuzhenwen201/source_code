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
import cn.fintechstar.admin.model.admin.message.BasicAdminAns;
import cn.fintechstar.admin.model.admin.message.BasicLoginAns;
import cn.fintechstar.admin.model.admin.message.BasicLogoutAns;
import cn.fintechstar.admin.model.admin.message.ChangePasswordAns;
import cn.fintechstar.admin.model.admin.message.DeleteAdminAns;
import cn.fintechstar.admin.model.admin.message.GetAdminsAns;
import cn.fintechstar.admin.service.AdminService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class AdminListener {
	
	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(AdminListener.class);
	
	@Autowired
	private RabbitTemplate rabbitTemplate;

	@Autowired
	private AdminService adminService;

	@RabbitListener(queues="ADMIN.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.RECONNECTION_REQ:
			BasicLoginAns reconnectAns = adminService.login(tNode.getBody());
			TNode reconnectAnsTnode = TNodeCodec.getTNode(Tag.RECONNECTION_ANS, GSON.toJson(reconnectAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + reconnectAns.getSgId(), new Message(TNodeCodec.encodeMessage(reconnectAnsTnode), new MessageProperties()));
			break;
		case Tag.LOGIN_REQ:
			BasicLoginAns loginAns = adminService.login(tNode.getBody());
			TNode loginAnsTnode = TNodeCodec.getTNode(Tag.LOGIN_ANS, GSON.toJson(loginAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + loginAns.getSgId(), new Message(TNodeCodec.encodeMessage(loginAnsTnode), new MessageProperties()));
			break;
		case Tag.LOGOUT_REQ:
			BasicLogoutAns logoutAns = adminService.logout(tNode.getBody());
			TNode logoutAnsTnode = TNodeCodec.getTNode(Tag.LOGIN_ANS, GSON.toJson(logoutAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + logoutAns.getSgId(), new Message(TNodeCodec.encodeMessage(logoutAnsTnode), new MessageProperties()));
			break;
		case Tag.FORCE_LOGOUT_REQ:
			BasicLogoutAns forceLogoutAns = adminService.logout(tNode.getBody());
			TNode forceLogoutAnsTnode = TNodeCodec.getTNode(Tag.LOGIN_ANS, GSON.toJson(forceLogoutAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + forceLogoutAns.getSgId(), new Message(TNodeCodec.encodeMessage(forceLogoutAnsTnode), new MessageProperties()));
			break;
		case Tag.CHANGE_PWD_REQ:
			ChangePasswordAns changePasswordAns = adminService.changePassword(tNode.getBody());
			TNode changePasswordAnsTnode = TNodeCodec.getTNode(Tag.CHANGE_PWD_ANS, GSON.toJson(changePasswordAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + changePasswordAns.getSgId(), new Message(TNodeCodec.encodeMessage(changePasswordAnsTnode), new MessageProperties()));
			break;
		case Tag.CREATE_ADMIN_ANS:
			BasicAdminAns createAdminAns = adminService.createAdmin(tNode.getBody());
			TNode createAdminAnsTnode = TNodeCodec.getTNode(Tag.CREATE_ADMIN_ANS, GSON.toJson(createAdminAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + createAdminAns.getSgId(), new Message(TNodeCodec.encodeMessage(createAdminAnsTnode), new MessageProperties()));
			break;
		case Tag.DELETE_ADMIN_REQ:
			DeleteAdminAns deleteAdminAns = adminService.deleteAdmin(tNode.getBody());
			TNode deleteAdminAnsTnode = TNodeCodec.getTNode(Tag.DELETE_ADMIN_ANS, GSON.toJson(deleteAdminAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + deleteAdminAns.getSgId(), new Message(TNodeCodec.encodeMessage(deleteAdminAnsTnode), new MessageProperties()));
			break;
		case Tag.UPDATE_ADMIN_REQ:
			BasicAdminAns updateAdminAns = adminService.updateAdmin(tNode.getBody());
			TNode updateAdminAnsTnode = TNodeCodec.getTNode(Tag.CREATE_ADMIN_ANS, GSON.toJson(updateAdminAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + updateAdminAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateAdminAnsTnode), new MessageProperties()));
			break;
		case Tag.GET_ADMINS_ANS:
			GetAdminsAns getAdminsAns = adminService.getAdmins(tNode.getBody());
			TNode getAdminsAnsTnode = TNodeCodec.getTNode(Tag.GET_ADMINS_ANS, GSON.toJson(getAdminsAns));
			rabbitTemplate.send(ResponseTitle.ADMIN_TITLE + getAdminsAns.getSgId(), new Message(TNodeCodec.encodeMessage(getAdminsAnsTnode), new MessageProperties()));
			break;
		default:
			LOGGER.error("unknown tag from LOGIN.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
}
