package com.broctagon.centrallogging.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.centrallogging.model.ErrorMessage;
import com.broctagon.centrallogging.send.MailSender;
import com.broctagon.tradtnode.rabbit.message.Message;
import com.fasterxml.jackson.databind.ObjectMapper;

@Service
public class MailService {
	
	@Autowired
	private MailSender mailSender;

	public void sendMails(List<Message> tnodeMessages) throws Exception {
		for(Message message : tnodeMessages){
			ErrorMessage errorMessage = new ObjectMapper().readValue(message.getMessageString(), ErrorMessage.class);
			mailSender.send(errorMessage.getBody(), message.getTag());
		}
	}

}
