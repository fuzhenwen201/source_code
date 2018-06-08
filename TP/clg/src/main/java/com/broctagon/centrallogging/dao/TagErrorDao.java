package com.broctagon.centrallogging.dao;

import com.broctagon.tradtnode.rabbit.message.Message;

public interface TagErrorDao {

	public void saveTagError(Message message);
	
}
