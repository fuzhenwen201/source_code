package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.LogoutDao;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class LogoutService {

	@Autowired
	private LogoutDao logoutDao;

	@Autowired
	private TagDao tagDao;

	public void addLogout(Message message) {
		if(Tag.LOGOUT_REQ == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			logoutDao.addLogoutReq(requestId, body);
		}else if (Tag.LOGOUT_ANS == message.getTag()) {
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			logoutDao.addLogoutAns(requestId, body);
		}else{
			tagDao.addTagError(message);
		}
	}
}
