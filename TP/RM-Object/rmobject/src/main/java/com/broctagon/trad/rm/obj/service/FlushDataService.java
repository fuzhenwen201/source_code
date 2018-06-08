/**
 * 
 */
package com.broctagon.trad.rm.obj.service;

import org.springframework.stereotype.Service;

/**
 * @author imdadullah
 *
 */
@Service
public interface FlushDataService {
	void deleteAllFromClosePositionTbl();
	void flushRedisData();
}
