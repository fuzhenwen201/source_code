package cn.fintechstar.traddc.config;

import java.util.HashMap;
import java.util.Map;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.Binding.DestinationType;
import org.springframework.amqp.core.Queue;
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
public class InstantMqBeanInfoRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor, EnvironmentAware{
	
	private ScopeMetadataResolver scopeMetadataResolver = new AnnotationScopeMetadataResolver();

	private String instantExchange;
	
	private Map<String, Object> queueMap = new HashMap<>();
	
	@Override
	public void setEnvironment(Environment environment) {
		instantExchange = environment.getProperty("mq.instantExchange");
		RelaxedPropertyResolver propertyResolver = new RelaxedPropertyResolver(environment, "mq.");
		queueMap = propertyResolver.getSubProperties("instantSignalQueue");
	}
	
	@Override
	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
		for(String key : queueMap.keySet()){
			String queueName = (String) queueMap.get(key);
			BeanDefinition queue = beanFactory.getBeanDefinition(queueName);
			ConstructorArgumentValues queueConstructor = queue.getConstructorArgumentValues();
			queueConstructor.addGenericArgumentValue(queueName);
			queueConstructor.addGenericArgumentValue(false);
			queueConstructor.addGenericArgumentValue(false);
			queueConstructor.addGenericArgumentValue(true);
			
			BeanDefinition binding1 = beanFactory.getBeanDefinition(queueName + "Binding1");
			ConstructorArgumentValues bindingConstructor1 = binding1.getConstructorArgumentValues();
			bindingConstructor1.addGenericArgumentValue(queueName);
			bindingConstructor1.addGenericArgumentValue(DestinationType.QUEUE);
			bindingConstructor1.addGenericArgumentValue(instantExchange);
			bindingConstructor1.addGenericArgumentValue(key.replace(".", ""));
			bindingConstructor1.addGenericArgumentValue(new HashMap<String,Object>());
		}
	}

	@Override
	public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
		for(String key : queueMap.keySet()){
			String queueName = (String) queueMap.get(key);
			registerBean(registry, queueName, Queue.class);
			registerBean(registry, queueName + "Binding1", Binding.class);
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
