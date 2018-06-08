package com.broctagon.trad.rm.db.dao;

import java.util.List;

import com.broctagon.trad.rm.db.model.Order;


public interface OrderDao {

	public int addOrders(List<Order> orders);
	
	public List<Order> queryAllOrders();
	public List<Order> queryAllOpenOrders();
	public List<Order> queryAllOpenOrdersInExpiryTime(long currentTime);
	public List<Order> queryAllClosedOrders();
	public List<Order> queryAllBalanceOrders();
	
	public List<Order> queryOrdersByClientId(Integer clientid);
	public List<Order> queryOrdersByStartOpenTime(long startTime);
	public List<Order> queryOrdersByStatus(int orderStatusttt);
	
	public List<Order> queryOpenOrdersByClientId(Integer clientid);
	public List<Order> queryClosedOrdersByClientId(Integer clientid);
	public List<Order> queryBalanceOrdersByClientId(Integer clientid);
	public List<Order> queryClosedAndBalanceOrdersByClientId(Integer clientid);
	public List<Order> queryLatest30ClosedAndBalanceOrdersByClientId(Integer clientid);
	
}
