package com.broctagon.centrallogging.rowmapper;


import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import com.broctagon.centrallogging.model.ErrorMessageVo;

@Component
public class ErrorMessageRowMapper implements RowMapper<ErrorMessageVo> {

	public ErrorMessageVo mapRow(ResultSet rs, int rowNum) throws SQLException {
		ErrorMessageVo errorMessage = new ErrorMessageVo();
		errorMessage.setId(rs.getInt("id"));
		errorMessage.setQueueName(rs.getString("queuename"));
		errorMessage.setTag(rs.getInt("tag"));
		errorMessage.setModule(rs.getString("module"));
		errorMessage.setErrno(rs.getInt("errno"));
		errorMessage.setErrmsg(rs.getString("errmsg"));	
//		errorMessage.setBody(new ObjectMapper().readValue(rs.getString("details"), Message.class));
		errorMessage.setDetails(rs.getString("details"));
		errorMessage.setTime(rs.getBigDecimal("time"));
		return errorMessage;
	}

}
