package com.broctagon.centrallogging.dao.impl;

import java.util.List;

import javax.sql.DataSource;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.broctagon.centrallogging.dao.ErrorMessageDao;
import com.broctagon.centrallogging.model.ErrorMessage;
import com.broctagon.centrallogging.model.ErrorMessageVo;
import com.broctagon.centrallogging.rowmapper.ErrorMessageRowMapper;

@Repository
@Transactional
public class ErrorMessageDaoImpl implements ErrorMessageDao {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;

	@Autowired
	private ErrorMessageRowMapper errorMessageRowMapper;

	
	public void saveErrorMessages(String sql, DataSource dataSource){
		if(!"BEGIN TRANSACTION;COMMIT;".equals(sql)){
			if(dataSource != null){
				jdbcTemplate.setDataSource(dataSource);
			}
			jdbcTemplate.update(sql);
		}
	}
	
	public void saveErrorMessage(String errorMessageType, ErrorMessage errorMessage, DataSource dataSource) {
		if(dataSource != null){
			jdbcTemplate.setDataSource(dataSource);
		}
		String sql = "INSERT INTO " + errorMessageType  + "(queuename, tag, time, module, errno, errmsg, details) VALUES (?, ?, ?, ?, ?, ?, ?);";
		jdbcTemplate.update(sql, new Object[]{errorMessage.getQueue(), errorMessage.getTag(), errorMessage.getTime(), errorMessage.getModule(), errorMessage.getErrno(), errorMessage.getErrmsg(), errorMessage.getBody().toString()});
	}

	public List<ErrorMessageVo> getAllErrorMessage(String errorMessageType, DataSource dataSource) {
		if(dataSource != null){
			jdbcTemplate.setDataSource(dataSource);
		}
		String sql = "SELECT id, queuename, tag, time, module, errno, errmsg, details FROM " + errorMessageType + ";";
		return jdbcTemplate.query(sql, errorMessageRowMapper);
	}

	public ErrorMessageVo getErrorMessageById(String errorMessageType, int id, DataSource dataSource) {
		if(dataSource != null){
			jdbcTemplate.setDataSource(dataSource);
		}
		String sql = "SELECT id, queuename, tag, time, module, errno, errmsg, details FROM " + errorMessageType + " where id = ?";
		return jdbcTemplate.queryForObject(sql, errorMessageRowMapper, id);
	}
	
}
