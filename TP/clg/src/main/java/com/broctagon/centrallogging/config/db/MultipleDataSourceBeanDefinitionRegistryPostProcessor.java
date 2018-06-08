package com.broctagon.centrallogging.config.db;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.sql.DataSource;

import org.springframework.beans.BeansException;
import org.springframework.beans.MutablePropertyValues;
import org.springframework.beans.factory.annotation.AnnotatedBeanDefinition;
import org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanDefinitionHolder;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.beans.factory.support.BeanDefinitionReaderUtils;
import org.springframework.beans.factory.support.BeanDefinitionRegistry;
import org.springframework.beans.factory.support.BeanDefinitionRegistryPostProcessor;
import org.springframework.beans.factory.support.BeanNameGenerator;
import org.springframework.boot.bind.RelaxedPropertyResolver;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.AnnotationBeanNameGenerator;
import org.springframework.context.annotation.AnnotationConfigUtils;
import org.springframework.context.annotation.AnnotationScopeMetadataResolver;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.ScopeMetadata;
import org.springframework.context.annotation.ScopeMetadataResolver;
import org.springframework.core.env.Environment;


@Configuration
public class MultipleDataSourceBeanDefinitionRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor, EnvironmentAware{

	private ScopeMetadataResolver scopeMetadataResolver = new AnnotationScopeMetadataResolver();
	
	private BeanNameGenerator beanNameGenerator = new AnnotationBeanNameGenerator();
	
	private static final Object DATASOURCE_TYPE_DAFAULT = "org.apache.tomcat.jdbc.pool.DataSource";
	
	private Map<String, Map<String, Object>> dataSourcesMap = new HashMap<String, Map<String,Object>>();
	
	
	public void setEnvironment(Environment environment) {
		RelaxedPropertyResolver propertyResolver = new RelaxedPropertyResolver(environment, "multi.datasource.");
		String datasourcePrefixs = propertyResolver.getProperty("names");
		String[] dataSourcePrefixArr = datasourcePrefixs.split(",");
		for(String dataSourcePrefix : dataSourcePrefixArr){
			Map<String, Object> dataSourceMap = propertyResolver.getSubProperties(dataSourcePrefix + ".");
			dataSourcesMap.put(dataSourcePrefix, dataSourceMap);
		}
	}

	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
		beanFactory.getBeanDefinition("dataSource").setPrimary(true);
		if(!dataSourcesMap.isEmpty()){
			BeanDefinition beanDefinition = null;
			Map<String, Object> dsMap = null;
			MutablePropertyValues mutablePropertyValues = null;
			for(Entry<String, Map<String, Object>> entry : dataSourcesMap.entrySet()){
				beanDefinition = beanFactory.getBeanDefinition(entry.getKey());
				mutablePropertyValues = beanDefinition.getPropertyValues();
				dsMap = entry.getValue();
				mutablePropertyValues.addPropertyValue("driverClassName", dsMap.get("driverClassName"));
				mutablePropertyValues.addPropertyValue("url", dsMap.get("url"));
//				mutablePropertyValues.addPropertyValue("username", dsMap.get("username"));
//				mutablePropertyValues.addPropertyValue("password", dsMap.get("password"));
			}
		}
	}

	@SuppressWarnings("unchecked")
	public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
		try{
			if (!dataSourcesMap.isEmpty()) {
				for(Entry<String, Map<String, Object>> entry : dataSourcesMap.entrySet()){
					Object type = entry.getValue().get("type");
					if(type == null){
						type = DATASOURCE_TYPE_DAFAULT;
					}
					registerBean(registry, entry.getKey(), (Class<? extends DataSource>)Class.forName(type.toString()));
				}
			}
		}catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	private void registerBean(BeanDefinitionRegistry registry, String name, Class<?> beanClass){
		AnnotatedBeanDefinition annotatedBeanDefinition = new AnnotatedGenericBeanDefinition(beanClass);
		ScopeMetadata scopeMetadata = this.scopeMetadataResolver.resolveScopeMetadata(annotatedBeanDefinition);
		annotatedBeanDefinition.setScope(scopeMetadata.getScopeName());
		String beanName = (name != null ? name : this.beanNameGenerator.generateBeanName(annotatedBeanDefinition, registry));
		AnnotationConfigUtils.processCommonDefinitionAnnotations(annotatedBeanDefinition);
		BeanDefinitionHolder definitionHolder = new BeanDefinitionHolder(annotatedBeanDefinition, beanName);
		BeanDefinitionReaderUtils.registerBeanDefinition(definitionHolder, registry);
	}

}
