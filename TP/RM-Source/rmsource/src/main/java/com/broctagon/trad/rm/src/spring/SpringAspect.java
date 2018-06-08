
package com.broctagon.trad.rm.src.spring;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import org.apache.log4j.Logger;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

/**
* @auther: Water
* @time: Oct 19, 2017 5:28:58 PM
* 
*/

@Aspect
@Component
public class SpringAspect {

	private Logger logger = Logger.getLogger(this.getClass());
		
	@Pointcut("execution(* com.broctagon.trad.rm.src.serviceimpl.*.*(..))")
	public void pointcut(){}
    
    @Before("pointcut()")
	public void doBefore(JoinPoint jp){	
				
		String className = jp.getTarget().getClass().getSimpleName();
		String methodName = jp.getSignature().getName();
		logger.info( "(" + className + ")(" + methodName + ")" );
		
		Object[] args = jp.getArgs();
		if(args!=null&&args.length>0){
			Object arg = args[0];
						
			byte[] bytes = null;  
			ObjectOutputStream oo = null;
            ByteArrayOutputStream bo = new ByteArrayOutputStream();          
	        try {  	  
	        	oo = new ObjectOutputStream(bo);  
	            oo.writeObject(arg);  	      
	            bytes = bo.toByteArray();  	      	            
	        } catch (Exception e) {  
	        	logger.error(e.getMessage());  
	            e.printStackTrace();  
	        }  
	        finally{
	        	try {
					bo.close();
				} catch (IOException e) {
					e.printStackTrace();
				}  
	        	if(oo!=null){
	        		try {
						oo.close();
					} catch (IOException e) {
						e.printStackTrace();
					}  
	        	}	            
	        }
	        	
	        if(bytes!=null){
	        	String argStr = new String(bytes);
	        	logger.info( "(" + className + ")(" + methodName + ")(content:" + argStr + ")" );
	        }			
		}
		
	}
	
	public Object doAround(){
		
		return null;
	}
	
	
}
