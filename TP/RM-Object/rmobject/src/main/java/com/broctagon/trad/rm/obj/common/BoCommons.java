package com.broctagon.trad.rm.obj.common;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import com.broctagon.trad.rm.db.model.TradeModel;
import com.broctagon.trad.rm.obj.message.BoScaleMsg;




public class BoCommons {
	public static List<BoScaleMsg> getScale(List<TradeModel> tlist) {
		List<BoScaleMsg> scalls = new ArrayList<BoScaleMsg>();
		Map<Long, List<TradeModel>> collect = tlist.stream().collect(Collectors.groupingBy(o -> o.getOpenprice()));
		collect.entrySet().forEach(e -> {
			List<TradeModel> trads = e.getValue();
			BoScaleMsg scall = new BoScaleMsg();
			scall.setNooftrade(trads.size());
			scall.setOpenprice(e.getKey());
			scall.setTotalvolume(trads.stream().mapToLong(o -> o.getAmount()).sum());
			scalls.add(scall);
		});
		return scalls;
	}
}
