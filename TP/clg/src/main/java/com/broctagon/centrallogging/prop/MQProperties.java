package com.broctagon.centrallogging.prop;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@ConfigurationProperties(prefix="mq")
@Component	
public class MQProperties {
	
	public static int circleQueueSize;

	private String host;
	
	private int port;
	
	private String username;
	
	private String password;
	
	private String vhost;
	
	private String exchange;
	
	private String errorQueue;
	
	private String errorRoutingKey;
	
	private int errorHeartBeats;
	
	private String heartBeatQueue;

	private String heartBeatRoutingKeyBase;
	
	private String heartBeatRoutingKey;
	
	private int mid;
	
	private String controlQueue;
	
	private String controlRoutingKey;
	

	public static int getCircleQueueSize() {
		return circleQueueSize;
	}

	public static void setCircleQueueSize(int circleQueueSize) {
		MQProperties.circleQueueSize = circleQueueSize;
	}

	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getVhost() {
		return vhost;
	}

	public void setVhost(String vhost) {
		this.vhost = vhost;
	}

	public String getExchange() {
		return exchange;
	}

	public void setExchange(String exchange) {
		this.exchange = exchange;
	}

	public String getErrorQueue() {
		return errorQueue;
	}

	public void setErrorQueue(String errorQueue) {
		this.errorQueue = errorQueue;
	}

	public String getErrorRoutingKey() {
		return errorRoutingKey;
	}

	public void setErrorRoutingKey(String errorRoutingKey) {
		this.errorRoutingKey = errorRoutingKey;
	}

	public int getErrorHeartBeats() {
		return errorHeartBeats;
	}

	public void setErrorHeartBeats(int errorHeartBeats) {
		this.errorHeartBeats = errorHeartBeats;
	}

	public String getHeartBeatQueue() {
		return heartBeatQueue;
	}

	public void setHeartBeatQueue(String heartBeatQueue) {
		this.heartBeatQueue = heartBeatQueue;
	}

	public String getHeartBeatRoutingKeyBase() {
		return heartBeatRoutingKeyBase;
	}

	public void setHeartBeatRoutingKeyBase(String heartBeatRoutingKeyBase) {
		this.heartBeatRoutingKeyBase = heartBeatRoutingKeyBase;
	}

	public String getHeartBeatRoutingKey() {
		return heartBeatRoutingKey;
	}

	public void setHeartBeatRoutingKey(String heartBeatRoutingKey) {
		this.heartBeatRoutingKey = heartBeatRoutingKey;
	}

	public int getMid() {
		return mid;
	}

	public void setMid(int mid) {
		this.mid = mid;
	}

	public String getControlQueue() {
		return controlQueue;
	}

	public void setControlQueue(String controlQueue) {
		this.controlQueue = controlQueue;
	}

	public String getControlRoutingKey() {
		return controlRoutingKey;
	}

	public void setControlRoutingKey(String controlRoutingKey) {
		this.controlRoutingKey = controlRoutingKey;
	}

}
