package cn.fintechstar.admin.config.rabbit;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.Binding.DestinationType;
import org.springframework.amqp.core.Queue;
import org.springframework.amqp.core.TopicExchange;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanDefinitionHolder;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.beans.factory.config.ConstructorArgumentValues;
import org.springframework.beans.factory.support.BeanDefinitionReaderUtils;
import org.springframework.beans.factory.support.BeanDefinitionRegistry;
import org.springframework.beans.factory.support.BeanDefinitionRegistryPostProcessor;
import org.springframework.boot.bind.RelaxedPropertyResolver;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.AnnotationConfigUtils;
import org.springframework.context.annotation.AnnotationScopeMetadataResolver;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.ScopeMetadata;
import org.springframework.context.annotation.ScopeMetadataResolver;
import org.springframework.core.env.Environment;

@Configuration
public class RabbitDetailsConfig implements BeanDefinitionRegistryPostProcessor, EnvironmentAware{
	
	private ScopeMetadataResolver scopeMetadataResolver = new AnnotationScopeMetadataResolver();
	
	private Map<String, Map<String, String>> queueOfExchangeMap = new HashMap<>();
	
	private Set<String> exchangeSet = new HashSet<>();
	
	@Override
	public void setEnvironment(Environment environment) {
		RelaxedPropertyResolver signalResolver = new RelaxedPropertyResolver(environment, "rabbit.");
		Map<String, Object> rabbitMap = signalResolver.getSubProperties("exchange.");
		for(String key : rabbitMap.keySet()){
			exchangeSet.add(key.split("\\.")[0]);
		}
		Map<String, String> queueMap = new HashMap<>();
		for(String key : rabbitMap.keySet()){
			String exchange = key.split("\\.")[0];
			if(queueOfExchangeMap.isEmpty() || queueOfExchangeMap.get(exchange) == null){
				queueMap = new HashMap<>();
			}else{
				queueMap = queueOfExchangeMap.get(exchange);
			}
			queueMap.put(key.split("\\.")[1] + ".AD", (String) rabbitMap.get(key));
			queueOfExchangeMap.put(exchange, queueMap);
		}
	}
	
	@Override
	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
		exchangeBeanGenerator(exchangeSet, beanFactory);
		queueBeanGenerator(queueOfExchangeMap, beanFactory);
		bindingBeanGenrator(queueOfExchangeMap, beanFactory);
	}

	private void queueBeanGenerator(Map<String, Map<String, String>> queueOfExchangeMap, ConfigurableListableBeanFactory beanFactory) {
		for(String key : queueOfExchangeMap.keySet()){
			Map<String, String> queueMap = queueOfExchangeMap.get(key);
			for(String queueName : queueMap.keySet()){
				BeanDefinition queue = beanFactory.getBeanDefinition(queueName);
				ConstructorArgumentValues queueConstructor = queue.getConstructorArgumentValues();
				queueConstructor.addGenericArgumentValue(queueName);
				queueConstructor.addGenericArgumentValue(false);
				queueConstructor.addGenericArgumentValue(false);
				queueConstructor.addGenericArgumentValue(true);
			}
		}
	}

	private void exchangeBeanGenerator(Set<String> exchangeSet, ConfigurableListableBeanFactory beanFactory) {
		for(String key: exchangeSet){
			BeanDefinition exchange = beanFactory.getBeanDefinition(key);
			ConstructorArgumentValues queueConstructor = exchange.getConstructorArgumentValues();
			queueConstructor.addGenericArgumentValue(key);
			queueConstructor.addGenericArgumentValue(false);
			queueConstructor.addGenericArgumentValue(false);
		}
	}
	
	private void bindingBeanGenrator(Map<String, Map<String, String>> queueOfExchangeMap, ConfigurableListableBeanFactory beanFactory) {
		for(String exchange : queueOfExchangeMap.keySet()){
			Map<String, String> queueMap = queueOfExchangeMap.get(exchange);
			for(String queueName : queueMap.keySet()){
				BeanDefinition binding = beanFactory.getBeanDefinition(queueName + "Binding");
				ConstructorArgumentValues bindingConstructor = binding.getConstructorArgumentValues();
				bindingConstructor.addGenericArgumentValue(queueName);
				bindingConstructor.addGenericArgumentValue(DestinationType.QUEUE);
				bindingConstructor.addGenericArgumentValue(exchange);
				bindingConstructor.addGenericArgumentValue(queueMap.get(queueName));
				bindingConstructor.addGenericArgumentValue(new HashMap<String,Object>());
			}
		}
	}


	@Override
	public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
		registerExchange(registry);
		registerQueue(registry);
		registerBinding(registry);
	}

	private void registerExchange(BeanDefinitionRegistry registry) {
		for(String exchangeName :  exchangeSet){
			registerBean(registry, exchangeName, TopicExchange.class);
		}
	}

	private void registerQueue(BeanDefinitionRegistry registry) {
		for(String key : queueOfExchangeMap.keySet()){
			Map<String, String> queueMap = queueOfExchangeMap.get(key);
			for(String queueName : queueMap.keySet()){
				registerBean(registry, queueName, Queue.class);
			}
		}
	}

	private void registerBinding(BeanDefinitionRegistry registry) {
		for(String key : queueOfExchangeMap.keySet()){
			Map<String, String> queueMap = queueOfExchangeMap.get(key);
			for(String queueName : queueMap.keySet()){
				registerBean(registry, queueName + "Binding", Binding.class);
			}
		}
	}

	private void registerBean(BeanDefinitionRegistry registry, String name, Class<?> beanClass) {
		AnnotatedGenericBeanDefinition annotatedGenericBeanDefinition = new AnnotatedGenericBeanDefinition(beanClass);
		ScopeMetadata scopeMetadata = this.scopeMetadataResolver.resolveScopeMetadata(annotatedGenericBeanDefinition);
		annotatedGenericBeanDefinition.setScope(scopeMetadata.getScopeName());
		
		AnnotationConfigUtils.processCommonDefinitionAnnotations(annotatedGenericBeanDefinition);
		BeanDefinitionHolder definitionHolder = new BeanDefinitionHolder(annotatedGenericBeanDefinition, name);
		BeanDefinitionReaderUtils.registerBeanDefinition(definitionHolder, registry);
	}

}
