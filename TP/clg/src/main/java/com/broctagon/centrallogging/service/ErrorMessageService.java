package com.broctagon.centrallogging.service;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.sql.DataSource;

import org.codehaus.jackson.map.ObjectMapper;
import org.springframework.amqp.core.Message;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.centrallogging.constant.ErrorMessageType;
import com.broctagon.centrallogging.dao.ErrorMessageDao;
import com.broctagon.centrallogging.model.ErrorMessage;
import com.broctagon.centrallogging.model.ErrorMessageVo;
import com.broctagon.centrallogging.util.TagUtil;
import com.broctagon.tradtnode.rabbit.message.MessageHelper;
import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;

@Service
@Transactional
public class ErrorMessageService {

	@Autowired
	private ErrorMessageDao errorMessageRepository;
	
	@Autowired
	private TagErrorService tagErrorService;
	
	public List<com.broctagon.tradtnode.rabbit.message.Message> getTnodeMessage(List<Object> messages) throws JsonParseException, JsonMappingException, IOException{
		List<com.broctagon.tradtnode.rabbit.message.Message> tnodeMessages = new ArrayList<com.broctagon.tradtnode.rabbit.message.Message>();
		for(Object message : messages){
			byte[] body = ((Message) message).getBody();
			if(body.length != 0){
				tnodeMessages.add(MessageHelper.parseMessage(body));
			}
		}
		return tnodeMessages;
	}
	
	public void saveErrorMessages(List<com.broctagon.tradtnode.rabbit.message.Message> messages, DataSource dataSource) throws JsonParseException, JsonMappingException, IOException{
		String sql = getInsertionSql(messages);
		errorMessageRepository.saveErrorMessages(sql, dataSource);
	}
	
	public void saveErrorMessage(String errorMessageType, ErrorMessage errorMessage, DataSource dataSource){
		errorMessageRepository.saveErrorMessage(errorMessageType, errorMessage, dataSource);
	}
	
	public List<ErrorMessageVo> getAllErrorMessage(String errorMessageType, DataSource dataSource){
		return errorMessageRepository.getAllErrorMessage(errorMessageType, dataSource);
	}
	
	public ErrorMessageVo getErrorMessageById(String errorMessageType, int id, DataSource dataSource){
		return errorMessageRepository.getErrorMessageById(errorMessageType, id, dataSource);
	}
	
	private String getInsertionSql(List<com.broctagon.tradtnode.rabbit.message.Message> messages) throws JsonParseException, JsonMappingException, IOException{
		StringBuffer sb = new StringBuffer();
		sb.append("BEGIN TRANSACTION;");
		for(com.broctagon.tradtnode.rabbit.message.Message message : messages){
				ErrorMessage errorMessage = new ObjectMapper().readValue(message.getMessageString(), ErrorMessage.class);
				sb.append("INSERT INTO ");
				if(TagUtil.clgTagSet.contains(message.getTag())){
					sb.append(getErrorMessage(errorMessage));
				}else{
					sb.append(tagErrorService.getTagError(message));
				}
		}
		sb.append("COMMIT;");
		return sb.toString();
	}
	
	private String getErrorMessage(ErrorMessage errorMessage) {
		StringBuffer sb = new StringBuffer();
		sb.append(ErrorMessageType.ERR_MSG).append("(queuename, tag, time, module, errno, errmsg, details)")
		.append(" VALUES('").append(errorMessage.getQueue()).append("', ")
		.append(errorMessage.getTag()).append(", ")
		.append(errorMessage.getTime()).append(", ")
		.append("'").append(errorMessage.getModule()).append("', ")
		.append(errorMessage.getErrno()).append(", ")
		.append("'").append(errorMessage.getErrmsg()).append("', ")
		.append("'").append(errorMessage.getBody().toString()).append("');");
		return sb.toString();
	}

}
