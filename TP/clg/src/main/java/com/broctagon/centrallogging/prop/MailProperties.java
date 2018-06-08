package com.broctagon.centrallogging.prop;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@ConfigurationProperties(prefix="mail")
@Component	
public class MailProperties {
	
	private String host;
	
	private String username;
	
	private String password;

	@Value("${mail.to.monitorGroup}")
	private String monitorGroup;
	
	@Value("${mail.to.singalGroup}")
	private String singalGroup;
	
	private String subject;
	

	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
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

	public String getMonitorGroup() {
		return monitorGroup;
	}

	public void setMonitorGroup(String monitorGroup) {
		this.monitorGroup = monitorGroup;
	}

	public String getSingalGroup() {
		return singalGroup;
	}

	public void setSingalGroup(String singalGroup) {
		this.singalGroup = singalGroup;
	}

	public String getSubject() {
		return subject;
	}

	public void setSubject(String subject) {
		this.subject = subject;
	}
	
}
