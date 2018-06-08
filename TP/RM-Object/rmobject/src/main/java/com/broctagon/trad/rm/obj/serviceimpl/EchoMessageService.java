package com.broctagon.trad.rm.obj.serviceimpl;

/**
 * @author imdadullah
 *
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.trad.rm.obj.common.CommonConstants;
import com.broctagon.trad.rm.obj.common.RMLogger;
import com.broctagon.trad.rm.obj.req.message.EchoReqMsg;
import com.broctagon.trad.rm.obj.res.message.EchoResMsg;
import com.broctagon.tradstnode.rabbit.message.Message;
import com.broctagon.tradstnode.rabbit.utils.ConstantUtils;
import com.broctagon.tradstnode.rabbit.utils.DateUtils;
import com.broctagon.tradstnode.rabbit.utils.JsonParserUtil;
import com.broctagon.tradstnode.rabbit.utils.StringUtil;


@Service
public class EchoMessageService {

	private Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private CommonPublisher commonPublisher;

	public void echoMessage(Message message, Boolean isTest) {
		String messageString = message.getMessageString();
		LOGGER.info("REQUEST FOR ECHO MSG ===== " + messageString);
		RMLogger.writeLogMessage(DateUtils.convertDateToString(new Date(), DateUtils.DdMmYyyyHmsFormatWithZone),
				ConstantUtils.MESSAGE_IN, message.getTag(), messageString.length(), messageString);
		EchoReqMsg echoReqMsg = JsonParserUtil.getObjectByJsonStr(messageString, EchoReqMsg.class);
		EchoResMsg echoResMsg = new EchoResMsg();
		echoResMsg.setReqid(echoReqMsg.getReqid());
		echoResMsg.setMid(echoReqMsg.getMid());
		echoResMsg.setTimestamp(echoReqMsg.getTimestamp());
		echoResMsg.setStatus("1");
		echoResMsg.setStarttimes(CommonUtil.rmStartTimeMap.get(CommonConstants.RM_START_TIME));
		try {
			List<String> commandOutPuts = getShellCommandOutPut();
			if (commandOutPuts != null) {
				echoResMsg.setName(commandOutPuts.get(0));
				echoResMsg.setPid(Integer.parseInt(commandOutPuts.get(1)));
				echoResMsg.setTimestamp(new Date(System.currentTimeMillis()).getTime());
			}
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
			echoResMsg.setStatus(null);
			LOGGER.error("ERROR MESSAGE : ", e);
		}
		commonPublisher.publishEchoMsgToSG(echoResMsg.getMid(), echoResMsg, isTest);
	}

	/**
	 * This method is Solution Getway work as Mediator between terminal and CM
	 */
	private List<String> getShellCommandOutPut() throws IOException, InterruptedException {
		// Build command
		List<String> commands = new ArrayList<>();
		commands.add("/bin/sh");
		commands.add("-c");
		commands.add("ps -eaf | grep RM_OBJ_V1.0.12");
		LOGGER.info(commands);
		// Run macro on target
		ProcessBuilder pb = new ProcessBuilder(commands);
		Process process = pb.start();

		// Read output
		List<String> readOutPut = new ArrayList<>();
		BufferedReader br = new BufferedReader(new InputStreamReader(process.getInputStream()));
		String line = null, previous = null;
		while ((line = br.readLine()) != null) {
			if (!line.equals(previous)) {
				previous = line;
				readOutPut.add(line);
				LOGGER.info(line);
			}
		}

		List<String> scriptShellOutput = null;
		for (String output : readOutPut) {
			if (output.contains("RM_OBJ_V1.0.12.jar")) {
				String[] strArray = StringUtil.split(output);
				scriptShellOutput = Arrays.asList(strArray);
				break;
			}
		}

		// Check and return result
		if (process.waitFor() == 0) {
			LOGGER.info("Success!!!");
			return scriptShellOutput;
		}

		// Abnormal termination: Log command parameters and output and throw
		// ExecutionException
		LOGGER.error(commands);
		LOGGER.error(readOutPut);
		// System.exit(1);
		return readOutPut;
	}
}
