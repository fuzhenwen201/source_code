package com.broctagon.centrallogging;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.sql.DataSource;

import org.assertj.core.api.Assertions;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.broctagon.centrallogging.constant.ErrorMessageType;
import com.broctagon.centrallogging.constant.Tag;
import com.broctagon.centrallogging.model.ErrorMessageVo;
import com.broctagon.centrallogging.service.ErrorMessageService;
import com.broctagon.tradtnode.rabbit.message.Message;
import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;


//@RunWith(SpringJUnit4ClassRunner.class)
//@SpringBootTest(classes=App.class)
public class ErrorMessageServiceTest {

	@Autowired
	private ErrorMessageService errorMessageService;
	
	@Autowired
	@Qualifier("ds1")
	private DataSource dataSource1;

	@Autowired
	@Qualifier("ds2")
	private DataSource dataSource2;
	
	
//	@Test
	public void getAllErrorMessageTest(){
		List<ErrorMessageVo> errorMessages = errorMessageService.getAllErrorMessage(ErrorMessageType.ERR_MSG, dataSource2);
		Assertions.assertThat(errorMessages.size()).isNotEqualTo(0);
	}
	
//	@Test
	public void getErrorMessageByIdTest(){
		ErrorMessageVo errorMessage = errorMessageService.getErrorMessageById(ErrorMessageType.ERR_MSG, 1, dataSource1);
		Assertions.assertThat(errorMessage).isNotNull();
	}
	
//	@Test
	public void saveErrorMessagesTest() throws JsonParseException, JsonMappingException, IOException{
		List<Message> messages = new ArrayList<Message>();
		Message message = new Message();
		message.setTag(Tag.MONITOR_TAG);
		String body = "{\"tag\":1, \"time\" : 1, \"queue\" : \"queuename\", \"module\" : \"clg\", \"errno\" : 4, \"errmsg\" : \"balance_error\", \"body\" :{\"reqid\" : \"null\", \"sessionid\" : \"null\", \"groupid\" : 0, \"clientid\" : 0, \"accountid\" : 1, \"balance\" : 1, \"prevbalance\" : 0, \"changed\" : 1, \"margin\" : 0, \"freemargin\" : 0, \"profit\" : 0, \"type\" : 0, \"closebalance\" : 0, \"status\" : 0}}";
		message.setMessageString(body);
		for(int i = 0 ; i < 10 ; i ++){
			messages.add(message);
		}
		errorMessageService.saveErrorMessages(messages, dataSource1);
	}
	
}
