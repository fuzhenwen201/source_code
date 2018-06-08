package com.broctagon.trad.rm.obj.res.message;

/**
 * 
 * @author imdadullah
 *
 */
public class EchoResMsg {
	private String reqid;
	private String status;
	private String pad;
	private String name;
	private int pid;
	private int mid;
	private long timestamp;
	private long starttimes;
	
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public String getStatus() {
		return status;
	}
	public void setStatus(String status) {
		this.status = status;
	}
	public String getPad() {
		return pad;
	}
	public void setPad(String pad) {
		this.pad = pad;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getPid() {
		return pid;
	}
	public void setPid(int pid) {
		this.pid = pid;
	}
	public int getMid() {
		return mid;
	}
	public void setMid(int mid) {
		this.mid = mid;
	}
	public long getTimestamp() {
		return timestamp;
	}
	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}
	public long getStarttimes() {
		return starttimes;
	}
	public void setStarttimes(long starttimes) {
		this.starttimes = starttimes;
	}
}
