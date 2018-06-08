package com.broctagon.trad.rm.obj.req.message;
/**
 * @author imdadullah
 *
 */
public class TopFiveBoReqMsg {
	private String reqid;
	private int managerid;
	private int direction;
	private int sgid;
	private String session;
	public String getSession() {
		return session;
	}
	public void setSession(String session) {
		this.session = session;
	}
	public String getReqid() {
		return reqid;
	}
	public void setReqid(String reqid) {
		this.reqid = reqid;
	}
	public int getManagerid() {
		return managerid;
	}
	public void setManagerid(int managerid) {
		this.managerid = managerid;
	}
	public int getDirection() {
		return direction;
	}
	public void setDirection(int direction) {
		this.direction = direction;
	}
	public int getSgid() {
		return sgid;
	}
	public void setSgid(int sgid) {
		this.sgid = sgid;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + direction;
		result = prime * result + managerid;
		return result;
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (!(obj instanceof TopFiveBoReqMsg))
			return false;
		TopFiveBoReqMsg other = (TopFiveBoReqMsg) obj;
		if (direction != other.direction)
			return false;
		if (managerid != other.managerid)
			return false;
		return true;
	}
	
	
}
