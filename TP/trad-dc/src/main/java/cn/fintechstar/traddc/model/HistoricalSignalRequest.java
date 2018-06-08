package cn.fintechstar.traddc.model;

public class HistoricalSignalRequest {
	
	private String reqId;
	private int groupId;
	private int clientId;
	private int candlesCount;;
	private Instrument instrument;
	private Granularity granularity;
	private int msgId;
	
	
	
	private byte[] sbe;
	private int len;
	
	public int getLen() {
		return len;
	}

	public void setLen(int len) {
		this.len = len;
	}

	public byte[] getSbe() {
		return sbe;
	}

	public void setSbe(byte[] sbe) {
		this.sbe = sbe;
	}
	
	

	public String getReqId() {
		return reqId;
	}
	
	public void setReqId(String reqId) {
		this.reqId = reqId;
	}
	
	public int getGroupId() {
		return groupId;
	}
	
	public void setGroupId(int groupId) {
		this.groupId = groupId;
	}
	
	public int getClientId() {
		return clientId;
	}
	
	public void setClientId(int clientId) {
		this.clientId = clientId;
	}
	
	public int getCandlesCount() {
		return candlesCount;
	}
	
	public void setCandlesCount(int candlesCount) {
		this.candlesCount = candlesCount;
	}
	
	public Instrument getInstrument() {
		return instrument;
	}
	
	public void setInstrument(Instrument instrument) {
		this.instrument = instrument;
	}
	
	public Granularity getGranularity() {
		return granularity;
	}
	
	public void setGranularity(Granularity granularity) {
		this.granularity = granularity;
	}

	public int getMsgId() {
		return msgId;
	}

	public void setMsgId(int msgId) {
		this.msgId = msgId;
	}

	@Override
	public String toString() {
		return "HistorySignalRequest [reqId=" + reqId + ", groupId=" + groupId + ", clientId=" + clientId
				+ ", candlesCount=" + candlesCount + ", instrument=" + instrument + ", granularity=" + granularity
				+ ", msgId=" + msgId + "]";
	}
	
	
}
