/**
 * 
 */
package com.broctagon.tradstnode.rabbit.utils;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.apache.log4j.Logger;

/**
 * This utility is for password generation which are md5 encrypted
 * @author kanhaiya.sahu
 *
 */
public class PasswordGeneratorUtil {
	
	private static Logger LOGGER = Logger.getLogger(PasswordGeneratorUtil.class);
	
    private static MessageDigest digest;
    
    /**
     * This method do the md5 hashing of the generated password
     * @param pwd
     * @return md5 pwd
     */
    public static String doMd5Hashing(String pwd){
    	try {
			digest = MessageDigest.getInstance("MD5");
			digest.reset();
		} catch (NoSuchAlgorithmException e) {
			LOGGER.error("Error Followed BY : ", e);
			e.printStackTrace();
		}
		digest.update(pwd.getBytes(), 0, pwd.length());
		String md5Str = new BigInteger(1, digest.digest()).toString(16);
		return md5Str;
    }
    
    public static void main(String[] args) {
		System.out.println(doMd5Hashing("Aa123$"));
		System.out.println(doMd5Hashing("h3Vt8kq6m+w="));
	}
    
}
