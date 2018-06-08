
package com.broctagon.trad.networkconfiguration.spring;

import org.apache.log4j.Logger;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.stereotype.Component;

/**
* @auther: Water
* @time: Oct 19, 2017 6:14:14 PM
* 
*/

@Aspect
@Component
public class SpringAspect {
	
	private Logger logger = Logger.getLogger(this.getClass());
	
	@Pointcut("execution(* com.broctagon.trad.networkconfiguration.serviceimpl.*ServiceImpl.updateConfigAndNotify(..))")
	public void pointCut(){}
	
	//@Before(value = "pointCut()")
	public void doBefore(JoinPoint jp){
		System.out.println("do before");
	}
	
	@Around(value = "pointCut()")
	public Object doAround(ProceedingJoinPoint pjp) throws Throwable{
				
		String className = pjp.getTarget().getClass().getSimpleName();
		String methodName = pjp.getSignature().getName();
		Object[] args = pjp.getArgs();
		logger.info("[" + className + "][" + methodName + "]");
		
		Object obj = pjp.proceed();
				
		return obj;		
	}

}
