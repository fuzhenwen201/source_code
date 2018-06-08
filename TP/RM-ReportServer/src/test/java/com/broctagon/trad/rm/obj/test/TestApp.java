package com.broctagon.trad.rm.obj.test;


import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import com.broctagon.trad.rm.obj.spring.SpringConfig;


public class TestApp {
	private static Logger logger = Logger.getLogger(TestApp.class);
	private static ApplicationContext ac;
	
    public static void main( String[] args )
    {    	
    	String configFilePath = "../configs/RMconfig.properties";
    	if(args.length>0){
    		configFilePath = args[0];
    	}
    	System.setProperty("configFilePath", configFilePath);
    	 	
    	try{
    	ac = new AnnotationConfigApplicationContext(SpringConfig.class);
    		
/*    		OrderDaoTest orderDaoTest = ac.getBean(OrderDaoTest.class);  		
     		orderDaoTest.InsertOrder();*/
    //		orderDaoTest.QueryOrder();   		
    	//	orderDaoTest.QueryOrdersByOrderStatus(132);;
     			
     		OnlineUserTest onlineUserTest = ac.getBean(OnlineUserTest.class);  	
     		onlineUserTest.getOnlineUserTest();
    	
    	}
    	catch(Exception ex){
    		ex.printStackTrace();
    		logger.info(ex.getMessage());
    	}
    	   	    	
        System.out.println( "Hello World!" );
    }
	
}
