/**
 * 
 */
package com.broctagon.tradstnode.rabbit.common;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.Scope;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.stereotype.Component;

/**
 * @author imdadullah
 *
 */
@Component
@Scope("singleton")
public class Listener {

	private static Logger LOGGER = Logger.getLogger(Listener.class);

	private static ApplicationContext appContext;

	/**
	 * This methods loads property file and bean.xml files also creates all the
	 * queues on rabbtMq and starts listening to that queues.
	 */
	public static void startListener() {
		try {
			LOGGER.error("<<<<<<<<<<<<<< GOING TO LOAD PROPERTY FILE >>>>>>>>>>>>>> ");
			String filePathValue = "../configs/config.properties";
			System.setProperty("configFilePath", filePathValue);
			appContext = new ClassPathXmlApplicationContext("bean.xml");
		} catch (Exception e) {
			LOGGER.error("ERROR FOLLOWED BY : ", e);
			e.printStackTrace();
		}

	}

	public static Object getPublisher(String senderBeanName) {
		return appContext.getBean(senderBeanName);
	}
}
