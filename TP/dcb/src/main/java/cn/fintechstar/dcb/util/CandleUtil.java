package cn.fintechstar.dcb.util;

import cn.fintechstar.dcb.model.Candle;

public class CandleUtil {

	public static Candle formatCandle(Candle candle) {
		candle.setOpenAsk(CommonUtil.doubleToDoubleByNumberFormat(candle.getOpenAsk()));
		candle.setOpenBid(CommonUtil.doubleToDoubleByNumberFormat(candle.getOpenBid()));
		candle.setHighAsk(CommonUtil.doubleToDoubleByNumberFormat(candle.getHighAsk()));
		candle.setHighBid(CommonUtil.doubleToDoubleByNumberFormat(candle.getHighBid()));
		candle.setLowAsk(CommonUtil.doubleToDoubleByNumberFormat(candle.getLowAsk()));
		candle.setLowBid(CommonUtil.doubleToDoubleByNumberFormat(candle.getLowBid()));
		candle.setCloseAsk(CommonUtil.doubleToDoubleByNumberFormat(candle.getCloseAsk()));
		candle.setCloseBid(CommonUtil.doubleToDoubleByNumberFormat(candle.getCloseBid()));
		return candle;
	}
}
