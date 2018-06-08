package com.broctagon.centrallogging.util;

import java.util.HashSet;
import java.util.Set;

import com.broctagon.centrallogging.constant.Tag;

public class TagUtil {
	
	public static Set<Short> clgTagSet = new HashSet<Short>();
	
	static{
		clgTagSet.add(Tag.MONITOR_TAG);
		clgTagSet.add(Tag.SIGNAL_TAG);
	}

}
