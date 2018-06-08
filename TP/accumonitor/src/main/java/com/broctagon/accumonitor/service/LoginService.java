package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.LoginDao;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class LoginService {

	@Autowired
	private LoginDao loginDao;
	
	@Autowired
	private TagDao tagDao;

	
	public void addLoginAns(Message message){
		if(Tag.LOGIN_REQ == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			loginDao.addLoginReq(requestId, body);
		}else if(Tag.LOGIN_ANS == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			loginDao.addLoginAns(requestId, body);
		}else{
			tagDao.addTagError(message);
		}
	}
}
