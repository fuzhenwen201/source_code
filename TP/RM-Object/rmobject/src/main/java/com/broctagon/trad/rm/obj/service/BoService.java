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
public interface BoService {

	public void fetchOrderAnsAndProcess();
	void processOrderListForUpdatingBo();
}
