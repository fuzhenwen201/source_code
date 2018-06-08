package cn.fintechstar.sendtest;

import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import com.rabbitmq.client.ConnectionFactory;

import cn.fintechstar.sendtest.perftest.MulticastParams;
import cn.fintechstar.sendtest.perftest.MulticastSet;
import cn.fintechstar.sendtest.show.PrintlnStats;
import cn.fintechstar.sendtest.util.ArgFormatUtil;

@SpringBootApplication
public class App {
	public static void main(String[] args) throws Throwable {
		Options options = getOptions();
		CommandLineParser parser = new GnuParser();
		try {
			CommandLine commandLine = parser.parse(options, args);
			if (commandLine.hasOption("?")) {
				showUsage(options);
				System.exit(0);
			}
			String testId = new SimpleDateFormat("HH:mm:ss-SSS").format(Calendar.getInstance().getTime());
			String host = ArgFormatUtil.strArg(commandLine, "h", "amqp://192.168.10.202");
			String username = ArgFormatUtil.strArg(commandLine, "u", "demo");
			String password = ArgFormatUtil.strArg(commandLine, "w", "demo");
			String vhost = ArgFormatUtil.strArg(commandLine, "v", "/demo");
			int heartBeat = ArgFormatUtil.intArg(commandLine, "h", 0);
			String exchangeType = ArgFormatUtil.strArg(commandLine, "t", "topic");
			String exchangeName = ArgFormatUtil.strArg(commandLine, "e", "signal_grp");
			String queueName = ArgFormatUtil.strArg(commandLine, "q", "DATAREQ.DATACENTRE");
			String routingKey = ArgFormatUtil.strArg(commandLine, "r", "DATAREQ.DATACENTRE");
			int proChannelCount = ArgFormatUtil.intArg(commandLine, "c", 1);
			int producers = ArgFormatUtil.intArg(commandLine, "p", 1);
			int proTranCount = ArgFormatUtil.intArg(commandLine, "x", 0);
			float proMessageRate = ArgFormatUtil.floatArg(commandLine, "l", 2000.0f);
			short tag = ArgFormatUtil.shortArg(commandLine, "g", (short)0xF003);
			String message = ArgFormatUtil.strArg(commandLine, "m", "{\"instrument\" : \"EUR_USD\",\"granularity\" : \"W\"}");
			
			PrintlnStats stats = new PrintlnStats(testId, 1000L);
			
			ConnectionFactory factory = new ConnectionFactory();
			factory.setHost(host);
			factory.setRequestedHeartbeat(heartBeat);
			factory.setUsername(username);
			factory.setPassword(password);
			factory.setVirtualHost(vhost);
			
			MulticastParams params = new MulticastParams();
			params.setAutoAck(false);
			params.setAutoDelete(true);
			params.setExchangeName(exchangeName);
			params.setExchangeType(exchangeType);
			params.setQueueName(queueName);
			params.setRoutingKey(routingKey);
			params.setProducerCount(producers);
			params.setProducerChannelCount(proChannelCount);
			params.setProducerTxSize(proTranCount);
			params.setProducerRateLimit(proMessageRate);
			params.setMessage(message);
			params.setTag(tag);
			
			MulticastSet set = new MulticastSet(stats, factory, params, host);
			set.run(true);
			
			stats.print();
		} catch (ParseException e) {
			System.err.println("fail to parse : " + e.getMessage());
		}catch (Exception e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	private static void showUsage(Options options) {
		HelpFormatter formatter = new HelpFormatter();
		formatter.printHelp("<program>", options);
	}

	private static Options getOptions() {
		Options options = new Options();
		options.addOption(new Option("?",   "help",		    		false,	 "show usage"));
		options.addOption(new Option("h",   "host",		        	true, 	 "connection uri"));
		options.addOption(new Option("u",	"username",				true,	 "username"));
		options.addOption(new Option("w",	"password",				true,	 "password"));
		options.addOption(new Option("v",	"vhost",				true,	 "vhost"));
		options.addOption(new Option("h", 	"heartbeat",			true,	 "heartbeat interval"));
		options.addOption(new Option("t",   "exchangeType", 		true, 	 "exchange type"));
		options.addOption(new Option("e",   "exchangeName", 		true,	 "exchange name"));
		options.addOption(new Option("q",   "queue",		 		true,	 "queue name"));
		options.addOption(new Option("r",   "routingKey", 			true,	 "routing key"));
		options.addOption(new Option("p",   "producers", 			true,	 "producer count"));
		options.addOption(new Option("c",   "proChannelCount",		true,	 "producer channel count"));
		options.addOption(new Option("x",   "proTranCount",			true, 	 "producer transaction size"));
		options.addOption(new Option("l",   "proRatelimit",         true,    "producer rate limit"));
		options.addOption(new Option("m",   "msgInfo", 				true,	 "message information"));
		options.addOption(new Option("g",	"tag",					true,	 "tag"));
		return options;
	}
}
