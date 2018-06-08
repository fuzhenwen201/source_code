package com.broctagon.trad.reportserver.test;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.sql.rowset.serial.SerialException;

import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import com.alibaba.fastjson.JSON;
import com.broctagon.trad.reportserver.config.SpringConfig;
import com.broctagon.trad.reportserver.model.Price;

import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

public class AppTest {

    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        
    	String configFilePath = "../configs/application.properties";
    	if(args.length>0){
    		configFilePath = args[0];
    	}
    	System.setProperty("configFilePath", configFilePath);
        
        try{
       	 ApplicationContext appContext = new AnnotationConfigApplicationContext(SpringConfig.class);
         ShardedJedisPool shardedJedisPool = appContext.getBean(ShardedJedisPool.class);
         ShardedJedis shardedRedis = shardedJedisPool.getResource();
         shardedRedis.set("testConfig", "valueWater");
         System.out.println("shardedRedis:" + shardedRedis.get("testConfig"));
         shardedRedis.close();
         for(String name: appContext.getBeanDefinitionNames()){
        	   System.out.println(name + ",");
         }
      
       }
       catch(Exception ex){
       	ex.printStackTrace();
       }
    }
  
}
