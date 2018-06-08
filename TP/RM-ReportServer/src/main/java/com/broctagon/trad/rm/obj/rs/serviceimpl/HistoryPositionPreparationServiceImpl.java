package com.broctagon.trad.rm.obj.rs.serviceimpl;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.rm.db.dao.OrderDao;
import com.broctagon.trad.rm.db.model.Order;
import com.broctagon.trad.rm.obj.common.CommonUtil;
import com.broctagon.trad.rm.obj.message.UpdateClosedPositionMsg;
import com.broctagon.trad.rm.obj.model.UpdateClosedPositionModel;
import com.broctagon.trad.rm.obj.rs.rabbitmq.RabbitMQHelper;
import com.broctagon.trad.rm.obj.rs.service.HistoryPositionPreparationService;

@Service
public class HistoryPositionPreparationServiceImpl implements HistoryPositionPreparationService{

	private Logger logger = Logger.getLogger(this.getClass());
	
	@Autowired private OrderDao orderDao;
	@Autowired private RabbitMQHelper rabbitMQHelper;
	
	public void prepareData(){
		
		List<Order> orderList = null;
		try{
			long startTime = CommonUtil.getIntradayStartNanos();
			orderList = orderDao.queryOrdersByStartOpenTime(startTime);
		}
		catch (ParseException e) {
			logger.info(e.getMessage());
		}
		catch(Exception ex){
			logger.info(ex.getMessage());
		}
		
		if(orderList!=null&&orderList.size()>0){				
			Stream<Order> orderStream = orderList.stream();
			
			Stream<Order> closedOrderStream = orderStream.filter(p -> (p.getCloseprice()>0));
			List<Order> closedOrders  = closedOrderStream.collect(Collectors.toList());
			if(closedOrders!=null&&closedOrders.size()>0){
				String closedOrdersStr = updateTraderClosedPosition(closedOrders);
				rabbitMQHelper.publishClosePositionAns(closedOrdersStr.getBytes());
			}
			
						
	//		Map<Integer, List<Order>> closeOrderAssetMap =  closedOrderStream.collect(Collectors.groupingBy(o -> o.getAssetid()));
			
	//		Stream<Order> openOrderStream = orderStream.filter(p -> (p.getCloseprice()==0 && p.getOpenprice()>0));
	//		List<Order> openOrders = openOrderStream.collect(Collectors.toList());
	//		Map<Integer, List<Order>> openOrderAssetMap =  openOrderStream.collect(Collectors.groupingBy(o -> o.getAssetid()));
					
			
	//		Stream<Order> openUpOrderStream = orderStream.filter(p -> (p.getCloseprice()==0 && p.getOpenprice()>0 && p.getDirection()==1));
	//		Map<Integer, List<Order>> openUpOrderAssetMap =  openUpOrderStream.collect(Collectors.groupingBy(o -> o.getAssetid()));
			
	//		Stream<Order> openDownOrderStream = orderStream.filter(p -> (p.getCloseprice()==0 && p.getOpenprice()>0 && p.getDirection()==0));
	//		Map<Integer, List<Order>> openDownOrderAssetMap =  openDownOrderStream.collect(Collectors.groupingBy(o -> o.getAssetid()));
				
		}
		else{
			logger.info("orderList is empty");
		}
	}
	
	private String updateTraderClosedPosition(List<Order> closeTradeList){
		UpdateClosedPositionModel updateAns = new UpdateClosedPositionModel();
		List<UpdateClosedPositionMsg> msgList =  new ArrayList<UpdateClosedPositionMsg>();
		for(Order order: closeTradeList){
			String orderId = order.getOrderid();
			UpdateClosedPositionMsg closePositionMsg = new UpdateClosedPositionMsg();
			closePositionMsg.setOrderid(orderId);
			msgList.add(closePositionMsg);
		}
		updateAns.setBody(msgList);
		String ansMsg = JSON.toJSONString(updateAns);
		return ansMsg;
	}
	
}
