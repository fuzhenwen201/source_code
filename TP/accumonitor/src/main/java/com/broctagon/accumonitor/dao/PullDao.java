package com.broctagon.accumonitor.dao;

public interface PullDao {

	public void addPullRequest(String requestId, String message);

	public void addPullResponse(String requestId, String message);

}
