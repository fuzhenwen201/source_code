package cn.fintechstar.dcb.constant;

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
	
	private String instantExchange;

	private Map<String, String> instantSignalQueue;
	
	
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

	public String getInstantExchange() {
		return instantExchange;
	}

	public void setInstantExchange(String instantExchange) {
		this.instantExchange = instantExchange;
	}

	public Map<String, String> getInstantSignalQueue() {
		return instantSignalQueue;
	}

	public void setInstantSignalQueue(Map<String, String> instantSignalQueue) {
		this.instantSignalQueue = instantSignalQueue;
	}
	
}
