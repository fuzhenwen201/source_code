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
import cn.fintechstar.admin.model.account.message.BasicAccountAns;
import cn.fintechstar.admin.model.account.message.GetAccountByIdAns;
import cn.fintechstar.admin.model.account.message.GetAccountsAns;
import cn.fintechstar.admin.service.AccountService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class AccountListener {

	private static final Gson GSON = new Gson();
	
	private static final Logger LOGGER = Logger.getLogger(AccountListener.class);
	
	@Autowired
	private AccountService accountService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;
	
	@RabbitListener(queues="ACCOUNT.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.CREATE_ACCOUNT_REQ:
			BasicAccountAns createAccountAns = accountService.createOrUpdateAccount(tNode.getBody());
			TNode createAccountAnsTnode = TNodeCodec.getTNode(Tag.CREATE_ACCOUNT_ANS, GSON.toJson(createAccountAns));
			rabbitTemplate.send(ResponseTitle.ACCOUNT_TITLE + createAccountAns.getSgId(), new Message(TNodeCodec.encodeMessage(createAccountAnsTnode),new MessageProperties()));
			break;
		case Tag.UPDATE_ACCOUNT_REQ:
			BasicAccountAns updateAccountAns = accountService.createOrUpdateAccount(tNode.getBody());
			TNode updateAccountAnsTnode = TNodeCodec.getTNode(Tag.UPDATE_ACCOUNT_ANS, GSON.toJson(updateAccountAns));
			rabbitTemplate.send(ResponseTitle.ACCOUNT_TITLE + updateAccountAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateAccountAnsTnode),new MessageProperties()));
			break;
		case Tag.GET_ACCOUNTS_REQ:
			GetAccountsAns getAccountsAns = accountService.getAccounts(tNode.getBody());
			TNode getAccountAnsTnode = TNodeCodec.getTNode(Tag.GET_ACCOUNTS_ANS, GSON.toJson(getAccountsAns));
			rabbitTemplate.send(ResponseTitle.ACCOUNT_TITLE + getAccountsAns.getSgId(), new Message(TNodeCodec.encodeMessage(getAccountAnsTnode), new MessageProperties()));
			break;
		case Tag.GET_ACCOUNT_BY_ID_REQ:
			GetAccountByIdAns getAccountByIdAns = accountService.getAccountById(tNode.getBody());
			TNode getAccountByIdAnsTnode = TNodeCodec.getTNode(Tag.GET_ACCOUNT_BY_ID_ANS, GSON.toJson(getAccountByIdAns));
			rabbitTemplate.send(ResponseTitle.ACCOUNT_TITLE + getAccountByIdAns.getSgId(), new Message(TNodeCodec.encodeMessage(getAccountByIdAnsTnode), new MessageProperties()));
			break;
		default:
			LOGGER.error("unknown tag from ACCOUNT.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
	
}
