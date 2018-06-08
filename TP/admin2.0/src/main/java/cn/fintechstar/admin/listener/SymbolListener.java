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
import cn.fintechstar.admin.model.symbol.message.BasicSymbolAns;
import cn.fintechstar.admin.model.symbol.message.GetSymbolByIdAns;
import cn.fintechstar.admin.model.symbol.message.GetSymbolsAns;
import cn.fintechstar.admin.service.SymbolService;
import cn.fintechstar.admin.tnode.TNode;
import cn.fintechstar.admin.tnode.TNodeCodec;

@Service
public class SymbolListener {
	
	private static final Logger LOGGER = Logger.getLogger(SymbolListener.class);
	
	private static final Gson GSON = new Gson();
	
	@Autowired
	private SymbolService symbolService;
	
	@Autowired
	private RabbitTemplate rabbitTemplate;

	@RabbitListener(queues="SYMBOL.AD")
	public void receiveMessage(Message message){
		TNode tNode = TNodeCodec.decodeMessage(message.getBody());
		switch (tNode.getTag()) {
		case Tag.CREATE_SYMBOL_REQ:
			BasicSymbolAns createSymbolAns = symbolService.createOrUpdateSymbol(tNode.getBody());
			TNode createSymbolAnsTNode = TNodeCodec.getTNode(Tag.CREATE_SYMBOL_ANS, GSON.toJson(createSymbolAns));
			rabbitTemplate.send(ResponseTitle.SYMBOL_TITLE + createSymbolAns.getSgId(), new Message(TNodeCodec.encodeMessage(createSymbolAnsTNode), new MessageProperties()));
			break;
		case Tag.UPDATE_SYMBOL_REQ:
			BasicSymbolAns updateSymbolAns = symbolService.createOrUpdateSymbol(tNode.getBody());
			TNode updateSymbolAnsTNode = TNodeCodec.getTNode(Tag.CREATE_SYMBOL_ANS, GSON.toJson(updateSymbolAns));
			rabbitTemplate.send(ResponseTitle.SYMBOL_TITLE + updateSymbolAns.getSgId(), new Message(TNodeCodec.encodeMessage(updateSymbolAnsTNode), new MessageProperties()));
			break;
		case Tag.GET_SYMBOLS_REQ:
			GetSymbolsAns getSymbolsAns = symbolService.getSymbols(tNode.getBody());
			TNode getSymbolsTNode = TNodeCodec.getTNode(Tag.GET_SYMBOLS_ANS, GSON.toJson(getSymbolsAns));
			rabbitTemplate.send(ResponseTitle.SYMBOL_TITLE + getSymbolsAns.getSgId(), new Message(TNodeCodec.encodeMessage(getSymbolsTNode), new MessageProperties()));
			break;
		case Tag.GET_SYMBOL_BY_ID_REQ:
			GetSymbolByIdAns getSymbolByIdAns = symbolService.getSymbolById(tNode.getBody());
			TNode getSymbolByIdTNode = TNodeCodec.getTNode(Tag.GET_SYMBOL_BY_ID_ANS, GSON.toJson(getSymbolByIdAns));
			rabbitTemplate.send(ResponseTitle.SYMBOL_TITLE + getSymbolByIdAns.getSgId(), new Message(TNodeCodec.encodeMessage(getSymbolByIdTNode), new MessageProperties()));
			break;
		case Tag.DELETE_SYMBOL_BY_ID_REQ:
			// TODO
			break;
		default:
			LOGGER.error("unknown tag from SYMBOL.AD.  tag:" + tNode.getTag() + "   --  message: " + tNode.getBody());
			break;
		}
	}
}
