package com.broctagon.trad.reportserver;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import com.broctagon.trad.reportserver.config.SpringConfig;

public class App {
	
	private static Logger logger = Logger.getLogger(App.class);
	
    public static void main( String[] args )
    {
          
    	String configFilePath = "../configs/application.properties";
    	if(args.length>0){
    		configFilePath = args[0];
    	}
    	System.setProperty("configFilePath", configFilePath);
        
        try{
        	ApplicationContext appContext = new AnnotationConfigApplicationContext(SpringConfig.class);
        }
        catch(Exception ex){
        	ex.printStackTrace();
        	logger.error(ex.getMessage());
        }
       
    }
}
