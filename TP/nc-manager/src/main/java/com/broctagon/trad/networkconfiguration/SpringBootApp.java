package com.broctagon.trad.networkconfiguration;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.scheduling.annotation.EnableScheduling;

//@EnableDiscoveryClient
@SpringBootApplication
@EnableScheduling
public class SpringBootApp {

	public static void main(String[] args){
		SpringApplication.run(SpringBootApp.class, args);
	}
	
}
