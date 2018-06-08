package cn.fintechstar.receivetest.prop;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@ConfigurationProperties(prefix="mq")
@Component
public class RabbitProp {

	private String host;
	
	private int port;
	
	private String username;
	
	private String password;
	
	private String vhost;
	
	private String exchange;
	
	private String reqQueue;
	
	private String reqRoutingKey;
	
	private String ansQueue;
	
	private String ansRoutingKey;

	
	
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

	public String getReqQueue() {
		return reqQueue;
	}

	public void setReqQueue(String reqQueue) {
		this.reqQueue = reqQueue;
	}

	public String getReqRoutingKey() {
		return reqRoutingKey;
	}

	public void setReqRoutingKey(String reqRoutingKey) {
		this.reqRoutingKey = reqRoutingKey;
	}

	public String getAnsQueue() {
		return ansQueue;
	}

	public void setAnsQueue(String ansQueue) {
		this.ansQueue = ansQueue;
	}

	public String getAnsRoutingKey() {
		return ansRoutingKey;
	}

	public void setAnsRoutingKey(String ansRoutingKey) {
		this.ansRoutingKey = ansRoutingKey;
	}
	
}
