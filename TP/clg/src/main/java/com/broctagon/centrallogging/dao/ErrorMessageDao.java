package com.broctagon.centrallogging.dao;

import java.util.List;

import javax.sql.DataSource;

import com.broctagon.centrallogging.model.ErrorMessage;
import com.broctagon.centrallogging.model.ErrorMessageVo;


public interface ErrorMessageDao {

	void saveErrorMessages(String sql, DataSource dataSource);
	
	void saveErrorMessage(String errorMessageType, ErrorMessage errorMessage, DataSource dataSource);
	
	List<ErrorMessageVo> getAllErrorMessage(String errorMessageType, DataSource dataSource);
	
	ErrorMessageVo getErrorMessageById(String errorMessageType, int id, DataSource dataSource);
}
