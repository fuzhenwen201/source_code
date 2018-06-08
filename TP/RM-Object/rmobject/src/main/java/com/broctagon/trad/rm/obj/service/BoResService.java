/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.obj.req.message.CustomBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.SymbolBoReqMsg;
import com.broctagon.trad.rm.obj.req.message.TopFiveBoReqMsg;

/**
 * @author imdadullah
 *
 */
@Service
public interface BoResService {
	String getTop5BoUpRes(TopFiveBoReqMsg req);
	String getTop5BoDownRes(TopFiveBoReqMsg req);
	String getCustomBoRes(CustomBoReqMsg req);
	String getSymbolBoRes(SymbolBoReqMsg req);
}
