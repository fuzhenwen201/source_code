package com.broctagon.centrallogging.dao.impl;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.centrallogging.dao.TagErrorDao;
import com.broctagon.tradtnode.rabbit.message.Message;

@Repository
@Transactional
public class TagErrorDaoImpl implements TagErrorDao {
	
	@Autowired
	@Qualifier("ds1")
	private DataSource dataSource;

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	public void saveTagError(Message message) {
		jdbcTemplate.setDataSource(dataSource);
		String sql = "INSERT INTO tag_error(responsequeue, tag, messagetype, body) VALUES(?, ?, ?, ?)";
		jdbcTemplate.update(sql, new Object[]{message.getResponceQueue(), message.getTag(), message.getMessageType(), message.getMessageString()});
	}

}
