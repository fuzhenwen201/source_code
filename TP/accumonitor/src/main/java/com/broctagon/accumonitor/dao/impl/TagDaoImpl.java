package com.broctagon.accumonitor.dao.impl;

import java.sql.PreparedStatement;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import com.broctagon.accumonitor.dao.TagDao;
import com.broctagon.tradtnode.rabbit.message.Message;

@Repository
public class TagDaoImpl implements TagDao{
	
	@Autowired
	private JdbcTemplate jdbvTemplate;

	private static final String ADD_TAG = "INSERT INTO TAG_ERROR(RESPONSE_QUEUE, TAG, MESSAGE_TYPE, BODY) VALUES(? ,? ,? ,?)";
	
	@Override
	public void addTagError(final Message message) {
		jdbvTemplate.update((con) ->{
			PreparedStatement ps = con.prepareStatement(ADD_TAG);
			ps.setString(1, message.getResponceQueue());
			ps.setInt(2, message.getTag());
			ps.setString(3, message.getMessageType() == null ? "" : message.getMessageType().toString());
			ps.setString(4, message.getMessageString());
			return ps;
		});
	}

}
