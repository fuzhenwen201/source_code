package cn.fintechstar.dc.constant;

import java.util.Map;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@Component
@ConfigurationProperties(prefix="mq")
public class RabbitProperties {

	private String host;
	
	private int port;
	
	private String username;

	private String password;
	
	private String vhost;
	
	private String signalExchange;
	
	private Map<String, String> queue;
	
	
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

	public String getSignalExchange() {
		return signalExchange;
	}

	public void setSignalExchange(String signalExchange) {
		this.signalExchange = signalExchange;
	}

	public Map<String, String> getQueue() {
		return queue;
	}

	public void setQueue(Map<String, String> queue) {
		this.queue = queue;
	}

}