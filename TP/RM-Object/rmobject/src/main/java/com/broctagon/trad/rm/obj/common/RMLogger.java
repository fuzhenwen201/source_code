package com.broctagon.trad.rm.obj.common;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;

import org.apache.log4j.Logger;

import com.broctagon.tradstnode.rabbit.utils.DateUtils;


public class RMLogger {
	
	private static Logger LOGGER = Logger.getLogger(RMLogger.class);
	
	/**
	 * method used to write change loge.
	 * request is the request string send by the client.
	 * requestTime is the time when request receive form the client.
	 * reply is the reply string send to the client
	 * replyTime is the time of sending reply to the client
	 * ManagerId is the manager id by which the client is login into the application.
	 */
	public void writeLogMessages(String request, String requestTime, String reply, String replayTime, int ManagerId){
		String line=ManagerId+"		"+requestTime+"	"+request+"	   "+replayTime+"	"+reply;
		write(line);
	}
	
	/*
	 * method used to write request loge.
	 * request is the request string send by the client.
	 * requestTime is the time when request receive form the client.
	 * reply is the reply string send to the client
	 * replyTime is the time of sending reply to the client
	 * ManagerId is the manager id by which the client is login into the application.
	 */
	public static void writeLogMessage(String logTime, String type, int tagId, int messageLength, String message) {
		String line = logTime + " [" + type + "], Tag:" + tagId + ", Length:" + messageLength + ", " + message;
		write(line);
	}

	
	
	/**
	 * this method will actually write into the log file.
	 * message is the message string to write into the file 
	 * fileName is the name of the file where logs will write
	 */
	private static void write(String message) {
		File dir = new File("../logs");
		dir.mkdirs();
		String fileName = "RM-" + DateUtils.convertDateToString(new Date(), "yyyy-MM-dd") + ".log";
		File logFile = new File(dir, fileName);
		if(!logFile.isFile()){
			try {
				logFile.createNewFile();
			} catch (IOException ex) {
				LOGGER.error("errorMessage = "+ ex.getMessage());
				ex.printStackTrace();
			}
		}
		
		BufferedWriter bw = null;
		
		try {
			bw = new BufferedWriter(new FileWriter(logFile.getAbsolutePath(), true));
			PrintWriter pwr = new PrintWriter(bw);
			pwr.println(message);
			pwr.close();
		} catch (IOException ex) {
			LOGGER.error("errorMessage = "+ ex.getMessage());
			ex.printStackTrace();
		} finally { 	// always close the file
			if (bw != null){
				try {
					bw.close();
				} catch (IOException ex) {
					LOGGER.error("errorMessage = "+ ex.getMessage());
					ex.printStackTrace();
				}
			}
		} 
	}
}
