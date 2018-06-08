package com.broctagon.trad.networkconfiguration.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class VerifierController {


	    
	 @RequestMapping("/test")
	 @ResponseBody
	 String homeTest() {
	    return "Hello World!";
	 }

}
