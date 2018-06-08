package com.broctagon.centrallogging.send;

import java.util.HashMap;
import java.util.Map;

import javax.mail.internet.MimeMessage;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.MimeMessageHelper;
import org.springframework.stereotype.Component;
import org.springframework.ui.freemarker.FreeMarkerTemplateUtils;
import org.springframework.util.StringUtils;
import org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer;

import com.broctagon.centrallogging.constant.Tag;
import com.broctagon.centrallogging.model.Message;
import com.broctagon.centrallogging.prop.MailProperties;
import com.broctagon.centrallogging.util.TagUtil;

import freemarker.template.Template;

@Component
public class MailSender {
	
	@Autowired
	private MailProperties mailProperties;

	@Autowired
	private JavaMailSender mailSender;

	@Autowired
	private FreeMarkerConfigurer freeMarkerConfigurer;
	
	public void send(Message message, short tag) throws Exception{
		MimeMessage mimeMessage = mailSender.createMimeMessage();
		MimeMessageHelper messageHelper = new MimeMessageHelper(mimeMessage, true);
		messageHelper.setFrom(mailProperties.getUsername());
		messageHelper.setTo("cherry@1stellar.com");
		messageHelper.setSubject(mailProperties.getSubject());
		Template template = freeMarkerConfigurer.getConfiguration().getTemplate("emailTemplate.html");
		Map<String, String> model = getModel(message);
		String html = FreeMarkerTemplateUtils.processTemplateIntoString(template, model);
		messageHelper.setText(html, true);
		mailSender.send(mimeMessage);
	}

	private Map<Short, String> getTagMap() {
		Map<Short, String> tagMap = new HashMap<Short, String>();
		tagMap.put(Tag.MONITOR_TAG, mailProperties.getMonitorGroup());
		tagMap.put(Tag.SIGNAL_TAG, mailProperties.getSingalGroup());
		return tagMap;
	}

	private Map<String, String> getModel(Message message) {
		Map<String, String> map = new HashMap<String, String>();
		map.put("reqId", message.getReqid());
		map.put("sessionId", message.getSessionid());
		map.put("groupId", String.valueOf(message.getGroupid()));
		map.put("clientId", String.valueOf(message.getClientid()));
		map.put("accountId", String.valueOf(message.getAccountid()));
		map.put("balance", String.valueOf(message.getBalance()));
		map.put("preBalance", String.valueOf(message.getPrevbalance()));
		map.put("changed", String.valueOf(message.getChanged()));
		map.put("margin", String.valueOf(message.getMargin()));
		map.put("freeMargin", String.valueOf(message.getFreemargin()));
		map.put("profit", String.valueOf(message.getProfit()));
		map.put("type", String.valueOf(message.getType()));
		map.put("closeBalance", String.valueOf(message.getClosebalance()));
		map.put("status", String.valueOf(message.getStatus()));
		return map;
	}

	public String[] getSendTo(String sendTo) {
		if(StringUtils.isEmpty(sendTo)){
			throw new RuntimeException("Mail Error: sendTo is empty!");
		}else{
			sendTo = sendTo.replaceAll(" ", "");
			if(sendTo.endsWith(";")){
				sendTo = sendTo.substring(0, sendTo.length() - 1);
			}
			/* multiple receivers */
			if(sendTo.contains(";")){
				return sendTo.split(";");
			}else{
				return new String[]{sendTo};
			}
		}
	}
	
}
