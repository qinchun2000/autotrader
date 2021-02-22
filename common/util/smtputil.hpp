#include <stdio.h>
#include <stdint.h>
#include <mimetic/mimetic.h>
#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "dateutil.hpp"


#ifndef _SMTPUTIL_HPP_
#define _SMTPUTIL_HPP_


class SmtpUtil
{
public:
	SmtpUtil(const char* staname,const char* str_to,const char* str_cc);
	static size_t my_read(void *ptr, size_t size, size_t nmemb, void *userp);
//	size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);

	int SendMail(std::string title,std::string content ,std::string pathfile);
	int SendMail_Content(std::string title,std::string content );
private:

	struct UserData
			{
			    std::stringstream ss;
			    size_t total;
			    UserData()
			        :total(0)
			    {

			    }
			};

//		const char *payload_text[] = {
//		  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
//		  "To: " TO "\r\n",
//		  "From: " FROM "(Example User)\r\n",
//		  "Cc: " CC "(Another example User)\r\n",
//		  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
//		  "rfcpedant.example.org>\r\n",
//		  "Subject: SMTP example message\r\n",
//		  "\r\n", /* empty line to divide headers from body, see RFC5322 */
//		  "The body of the message starts here.\r\n",
//		  "\r\n",
//		  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
//		  "Check RFC5322.\r\n",
//		  NULL
//		};

	struct upload_status {
	  int lines_read;
	};

	std::string _email_address;
	std::string _email_password;
	std::string _smtp_address;
	std::string _email_to;
	std::string _email_cc;

	int BUFFERR_SIZE =4096;


};

#endif /* _SMTPUTIL_HPP_ */
