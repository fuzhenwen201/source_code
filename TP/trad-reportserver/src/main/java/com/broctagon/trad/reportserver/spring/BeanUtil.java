package com.broctagon.trad.reportserver.spring;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;

@Component
public class BeanUtil implements ApplicationContextAware{
	
	private static ApplicationContext ac;

	public void setApplicationContext(ApplicationContext appContext) throws BeansException {
		ac = appContext;
	}
	
	public static ApplicationContext getApplicationContext(){
		return ac;
	}
	
	public static Object getBean(String paramStr){
		
		return ac.getBean(paramStr);
	}
	
	public static Object getBean(Class cls){
		
		return ac.getBean(cls);
	}

}
