package com.broctagon.trad.rm.db;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@ComponentScan(basePackages="com.broctagon.trad.rm")
public class SpringBootApp {

	public static void main(String[] args){
		SpringApplication.run(SpringBootApp.class, args);
	}
	
}
