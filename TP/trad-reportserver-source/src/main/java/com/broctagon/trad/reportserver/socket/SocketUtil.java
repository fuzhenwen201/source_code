package com.broctagon.trad.reportserver.socket;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.common.CommonUtil;
import com.broctagon.trad.reportserver.model.TradersAns;
import com.broctagon.trad.reportserver.model.Trader;

public class SocketUtil {
	
	private static Logger logger = Logger.getLogger(SocketUtil.class);
	
	public static final int headLength = 8;
	
	public static void testTraderList(){
		List<Trader> traderlist = new ArrayList<Trader>();
		Trader t1 = new Trader();
		t1.setAccountid(123);
		t1.setAddress("address123");
		Trader t2 = new Trader();
		t2.setAccountid(124);
		t2.setAddress("address124");
		traderlist.add(t1);
		traderlist.add(t2);
		
		TradersAns ans = new TradersAns();
		ans.setReqid("1234");
		ans.setTraderlist(JSON.toJSONString(traderlist));
		
		String ansStr = JSON.toJSONString(ans);
		System.out.println("ansStr="+ansStr);
	}

	public static String readBodyStr(InputStream inputStream) throws IOException{
		
		byte[] head = new byte[headLength];			
		inputStream.read(head);	
					
		int bodyLength = CommonUtil.bytesToIntBig(head, 4);
		// System.out.println("bodyLength=" + bodyLength);
		
		byte[] body = new byte[bodyLength];
		byte[] readContent = new byte[4096];
		int totalCount=0;
		int readCount=0;
		
		while(bodyLength-totalCount>0){
			if(bodyLength-totalCount>readContent.length){
				readCount = inputStream.read(body,totalCount,readContent.length);
			//	System.out.println("readCount1=" + readCount);
			//	System.out.println("totalCount1=" + totalCount);
			}
			else{
				readCount = inputStream.read(body,totalCount,bodyLength-totalCount);
			//	System.out.println("readCount2=" + readCount);
			//	System.out.println("totalCount2=" + totalCount);
		
			}
			totalCount += readCount;
		}
				
		String bodyStr = new String(body);		
		return bodyStr;
	}
	
	public static void sendGetTradersReq(){
		
	}
	
	public static byte[] prepareEntireBody(short tag, String sourceBody){
		
		sourceBody=sourceBody+'\0';
		byte[] bodyByte = sourceBody.getBytes();		
		byte[] outByte = new byte[bodyByte.length+headLength];
					
		outByte[0] = (byte)0x8f;
		
		byte[] shortByte = CommonUtil.shortToBytesBig(tag);			
		outByte[2] = shortByte[0];
		outByte[3] = shortByte[1];
		byte[] intByte = CommonUtil.intToBytesBig(bodyByte.length);
		outByte[4] = intByte[0];
		outByte[5] = intByte[1];
		outByte[6] = intByte[2];
		outByte[7] = intByte[3];
		
		for(int i=0; i<bodyByte.length;i++){
			outByte[headLength+i] = bodyByte[i];
		}
		
		return outByte;
		
	}
	
}
