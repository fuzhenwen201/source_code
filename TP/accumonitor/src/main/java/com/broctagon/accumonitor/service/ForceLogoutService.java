package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.ForceLogoutDao;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class ForceLogoutService {
	
	@Autowired
	private ForceLogoutDao forceLogoutDao;

	@Autowired
	private TagDao tagDao;
	
	public void addForceLogout(Message message) {
		if(Tag.FORCELOGOUT_REQ == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			forceLogoutDao.addForceLogoutReq(requestId, body);
		}else if (Tag.FORCELOGOUT_ANS == message.getTag()) {
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			forceLogoutDao.addForceLogoutAns(requestId, body);
		}else {
			tagDao.addTagError(message);
		}
	}

}
