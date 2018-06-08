package com.broctagon.accumonitor.dao;

import com.broctagon.tradtnode.rabbit.message.Message;

public interface TagDao {

	void addTagError(Message message);
	
}
