/**
 * 
 */
package com.broctagon.trad.rm.obj.serviceimpl;

import java.sql.Timestamp;
import java.util.List;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.broctagon.trad.rm.db.dao.CloseTradeDao;
import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.common.CommonPublisher;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.trad.rm.obj.model.ClosedTradDaoAdminModel;
import com.broctagon.trad.rm.obj.service.ClosedTradDaoAdminService;
import com.broctagon.trad.rm.obj.service.FlushDataService;


/**
 * @author imdadullah
 *
 */
@Service
public class ClosedTradDaoAdminServiceImp implements ClosedTradDaoAdminService {
	Logger LOGGER = Logger.getLogger(this.getClass());
	@Autowired
	private CloseTradeDao closeTradeDao;
	@Autowired
	private CommonPublisher commonPublisher;
	@Autowired
	private FlushDataService flushDataService;

	public void processAndSend() {
		try {
			LOGGER.info("********GOING TO UPDATE ADMIN DB*********");
			List<TradeModel> tradeList = getClosedTradeList();
			int tradeListSize = tradeList.size();
			if (tradeListSize >= 20) {
				int loopNo = (tradeListSize / 20);
				for (int i = 0; i < loopNo; i++) {
					ClosedTradDaoAdminModel tradeListModel = new ClosedTradDaoAdminModel();
					List<TradeModel> tradesubList = tradeList.subList(0, 20);
					tradeListModel.setBody(tradesubList);
					tradeListModel.setTimeDuration(new Timestamp(System.currentTimeMillis()));
					commonPublisher.publishClosePositionToAdmin(tradeListModel);
					tradeList.subList(0, 20).clear();
				}
			} else {
				if (tradeListSize > 0) {
					ClosedTradDaoAdminModel tradeListModel = new ClosedTradDaoAdminModel();
					tradeListModel.setBody(tradeList);
					tradeListModel.setTimeDuration(new Timestamp(System.currentTimeMillis()));
					commonPublisher.publishClosePositionToAdmin(tradeListModel);
				}

			}
			if (CommonUtil.flushFlag) {
				LOGGER.info("######## CLOSED TRAD LIST SIZE [ " + tradeList.size() + "   ] IS GOING TO DELETE ######");
				CommonUtil.serviceFlag = false;
				try {
					flushDataService.deleteAllFromClosePositionTbl();
					flushDataService.flushRedisData();
					CommonUtil.flushFlag = false;
				} catch (Exception e) {
					LOGGER.error("ERROR FOLLOWED BY  : ", e);
					e.printStackTrace();
				} finally {
					CommonUtil.serviceFlag = true;
				}
			}
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY  : ", e);
			e.printStackTrace();
		}

	}

	private List<TradeModel> getClosedTradeList() {
		List<TradeModel> trades = closeTradeDao.selectCloseTradesByTime();
		return trades;
	}

}
