package com.broctagon.trad.rm.obj.common;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.stereotype.Service;

@Service
public class CommonUtil {
private static Logger LOGGER = Logger.getLogger(CommonUtil.class);
	public static boolean  flushFlag = false;
	public static boolean serviceFlag = true;
	public static Map<String, Long> rmStartTimeMap = new HashMap<>();
	
	public static void checkServicePermission(){
		if(!CommonUtil.serviceFlag){
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				LOGGER.error("ERROR FOLLOWED BY  : ", e);
				e.printStackTrace();
			}
		}
	}

	public static long getIntradayStartNanos() throws ParseException{
		Date date = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
		String dateStr = sdf.format(date);
	//	System.out.println("dateStr:" + dateStr);	
		
		Date intradayStart = sdf.parse(dateStr);
		long intradayStartNanos = intradayStart.getTime()*1000;
		System.out.println("intradayStartNanos:" + intradayStartNanos);	
		return intradayStartNanos;
	}
	
	public static boolean isEmptyString(String str){
		if(str==null||"".equals(str.trim())){
			return true;
		}
		else{
			return false;
		}
	}
	
	public static byte[] intToBytesLittle( int value )   
	{   
	    byte[] src = new byte[4];  
	    src[3] =  (byte) ((value>>24) & 0xFF);  
	    src[2] =  (byte) ((value>>16) & 0xFF);  
	    src[1] =  (byte) ((value>>8) & 0xFF);    
	    src[0] =  (byte) (value & 0xFF);       	    
	    return src;   
	}  
	
	public static byte[] intToBytesBig( int value )   
	{   
	    byte[] src = new byte[4];  
	    src[0] =  (byte) ((value>>24) & 0xFF);  
	    src[1] =  (byte) ((value>>16) & 0xFF);  
	    src[2] =  (byte) ((value>>8) & 0xFF);    
	    src[3] =  (byte) (value & 0xFF);       	    
	    return src;   
	}  
	
	public static byte[] shortToBytesLittle( short value )   
	{   
	    byte[] src = new byte[2];  
	    src[1] =  (byte) ((value>>8) & 0xFF);    
	    src[0] =  (byte) (value & 0xFF);       	    
	    return src;   
	}  
	
	public static byte[] shortToBytesBig( short value )   
	{   
	    byte[] src = new byte[2];  
	    src[0] =  (byte) ((value>>8) & 0xFF);    
	    src[1] =  (byte) (value & 0xFF);       	    
	    return src;   
	}
	
/*	public static byte[] shortToBytesLittle( short value )   
	{   
	    byte[] src = new byte[2];  
	    src[1] =  (byte) ((value>>8) & 0xFF);    
	    src[0] =  (byte) (value & 0xFF);       	    
	    return src;   
	}  
	
	public static byte[] shortToBytesBig( short value )   
	{   
	    byte[] src = new byte[2];  
	    src[0] =  (byte) ((value>>8) & 0xFF);    
	    src[1] =  (byte) (value & 0xFF);       	    
	    return src;   
	}*/
	
	
	public static int bytesToIntLittle(byte[] src, int offset) {  
	    int value;    
	    value = (int) ((src[offset] & 0xFF)   
	            | ((src[offset+1] & 0xFF)<<8)   
	            | ((src[offset+2] & 0xFF)<<16)   
	            | ((src[offset+3] & 0xFF)<<24));  
	    return value;  
	}  
	
	public static int bytesToIntBig(byte[] src, int offset) {  
	    int value;    
	    value = (int)( ((src[offset] & 0xFF)<<24)   
	            | ((src[offset+1] & 0xFF)<<16)   
	            | ((src[offset+2] & 0xFF)<<8)   
	            | (src[offset+3] & 0xFF) );  
	    return value;  
	} 
	

	
	public static short bytesToShortLittle(byte[] src, int offset) {  
		short value;    
	    value = (short) ((src[offset] & 0xFF)   
	            | ((src[offset+1] & 0xFF)<<8) );  
	    return value;  
	}  
	
	public static short bytesToShortBig(byte[] src, int offset) {  
	    short value;    
	    value = (short)( ((src[offset] & 0xFF)<<8)   
	            | (src[offset+1] & 0xFF) );  
	    return value;  
	}
		
	public static String bytesToHexString(byte[] src){  
	    StringBuilder stringBuilder = new StringBuilder("");  
	    if (src == null || src.length <= 0) {  
	        return null;  
	    }  
	    for (int i = 0; i < src.length; i++) {  
	        int v = src[i] & 0xFF;  
	        String hv = Integer.toHexString(v);  
	        if (hv.length() < 2) {  
	            stringBuilder.append(0);  
	        }  
	        stringBuilder.append(hv);  
	    }  
	    return stringBuilder.toString();  
	}  
	
}