/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import org.springframework.stereotype.Service;

import com.broctagon.tradstnode.rabbit.message.Message;

/**
 * @author imdadullah
 *
 */
@Service
public interface BoReqService {
	void addTopFive5BoReq(String boReqMsgStr);
	void unSubTopFive5BoReq(String unsubMsgStr);
	void addCustomBoReq(String customReqMsgStr);
	void unSubCustomBoReq(String unsubMsgStr);
	void addSymbolBoReq(Message message);
	void unSubSymbolBoReq(Message message);
}
