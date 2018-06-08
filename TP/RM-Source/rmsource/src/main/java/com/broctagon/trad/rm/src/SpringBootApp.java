package com.broctagon.trad.rm.src;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
@ComponentScan(basePackages="com.broctagon.trad.rm")
public class SpringBootApp implements CommandLineRunner{

	public static void main(String[] args){
		SpringApplication.run(SpringBootApp.class, args);
	}

	@Override
	public void run(String... arg0) throws Exception {
		System.out.println("RUN CALLED");
		
	}
	
}
