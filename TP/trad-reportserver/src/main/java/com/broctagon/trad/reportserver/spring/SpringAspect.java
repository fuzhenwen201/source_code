package com.broctagon.trad.reportserver.spring;

import java.util.List;

import org.apache.log4j.Logger;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.springframework.stereotype.Component;

@Component("springAspect")
public class SpringAspect {

	private Logger logger = Logger.getLogger(SpringAspect.class);
	
	public Object doAddAround(ProceedingJoinPoint pjp) throws Throwable{
		
		long start = System.currentTimeMillis();

		Object retObj = pjp.proceed();
		
		String className = pjp.getTarget().getClass().getSimpleName();
		String methodName = pjp.getSignature().getName();		
		int count = -1;
		Object[] argArr = pjp.getArgs();
		if(argArr!=null&&argArr.length>0){
			Object argObj = pjp.getArgs()[0];
			if(argObj!=null){
				List argList = (List)argObj;
				count = argList.size();
			}
		}
				
		long end = System.currentTimeMillis();
		long duration = end - start;
		logger.info( "(" + className + ")(" + methodName + ")(listCount:" + count + ") duration:" + duration );
		
		return retObj;
	}
	
	public Object doWriteAround(ProceedingJoinPoint pjp) throws Throwable{		
		
		long start = System.currentTimeMillis();
		
		Object retObj = pjp.proceed();
		
		String className = pjp.getTarget().getClass().getSimpleName();
		String methodName = pjp.getSignature().getName();	
		
		long end = System.currentTimeMillis();
		long duration = end - start;
		logger.info( "(" + className + ")(" + methodName + ") duration:" + duration );
		
		return retObj;
	}
	
	public void doBefore(JoinPoint jp){		
		long start = System.currentTimeMillis();
		System.out.println("aspect before:" + start);
		logger.info("aspect before:" + start);
		
	}
	
	public void doAfter(JoinPoint jp){
		long start = System.currentTimeMillis();
		System.out.println("aspect after:" + start);
	}
	
}
