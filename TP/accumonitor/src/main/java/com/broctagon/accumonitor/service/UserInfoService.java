package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.dao.UserInfoDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class UserInfoService {

	@Autowired
	private UserInfoDao userInfoDao;	
	
	@Autowired
	private TagDao tagDao;

	
	public void addUserInfo(Message message){
		if(Tag.USER_INFO == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			userInfoDao.addUserInfo(requestId, body);
		}else{
			tagDao.addTagError(message);
		}
	}
}
