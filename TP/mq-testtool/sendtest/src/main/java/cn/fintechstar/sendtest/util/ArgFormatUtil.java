package cn.fintechstar.sendtest.util;

import org.apache.commons.cli.CommandLine;

public class ArgFormatUtil {

	public static float floatArg(CommandLine commandLine, String opt, float def) {
		return Float.parseFloat(commandLine.getOptionValue(opt, Float.toString(def)));
	}

	public static int intArg(CommandLine commandLine, String opt, int def) {
		return Integer.parseInt(commandLine.getOptionValue(opt, Integer.toString(def)));
	}

	public static String strArg(CommandLine commandLine, String opt, String def) {
		return commandLine.getOptionValue(opt, def);
	}

	public static short shortArg(CommandLine commandLine, String opt, short def) {
		return Short.parseShort(commandLine.getOptionValue(opt, String.valueOf(def)));
	}

}
