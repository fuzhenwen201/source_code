package com.broctagon.trad.reportserver.config;


import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;

@Configuration
@ComponentScan(basePackages="com.broctagon.trad.reportserver")
@PropertySource("file:${configFilePath}")
public class SpringConfig {
	

}
