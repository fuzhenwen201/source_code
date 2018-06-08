package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.PullDao;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class PullService {
	
	@Autowired
	private PullDao pullDao;
	
	@Autowired
	private TagDao tagDao;

	public void addPull(Message message) {
		if(Tag.SYNC_PULL == message.getTag()){
			String body = message.getMessageString();
			if("32".equals(ConstantUtil.getMessageFromRequest(body, "type"))){
				String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
				pullDao.addPullRequest(requestId, body);
			}
		}else if(Tag.RES_PULL == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			pullDao.addPullResponse(requestId, body);
		}else{
			tagDao.addTagError(message);
		}
	}

}
