package com.broctagon.accumonitor.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.accumonitor.constant.Tag;
import com.broctagon.accumonitor.dao.OCPositionDao;
import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.accumonitor.util.ConstantUtil;
import com.broctagon.tradtnode.rabbit.message.Message;

@Service
public class OCPositionService {
	
	@Autowired
	private OCPositionDao ocPositionDao;
	
	@Autowired
	private TagDao tagDao;

	public void addOCPosition(Message message){
		if(Tag.OC_POSITION == message.getTag()){
			String body = message.getMessageString();
			String requestId = ConstantUtil.getMessageFromRequest(body, "reqid");
			ocPositionDao.addOCPosition(requestId, body);
		}else{
			tagDao.addTagError(message);
		}
	}
}
