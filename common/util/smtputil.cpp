#include "smtputil.hpp"

SmtpUtil::SmtpUtil(const char* staname,const char* str_to,const char* str_cc)
{
	Config config(staname);
	this->_email_address=config.GetEmailAddress();
	this->_email_password=config.GetEmailPasswrod();
	this->_smtp_address=config.GetSmtpAddress();
	this->_email_to= str_to;
	this->_email_cc= str_cc;
}


//size_t SmtpUtil::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
//{
//  struct upload_status *upload_ctx = (struct upload_status *)userp;
//  const char *data;
//
//  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
//    return 0;
//  }
//
//  data = payload_text[upload_ctx->lines_read];
//
//  if(data) {
//    size_t len = strlen(data);
//    memcpy(ptr, data, len);
//    upload_ctx->lines_read++;
//
//    return len;
//  }
//
//  return 0;
//}


size_t SmtpUtil::my_read(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct UserData * pstream = static_cast<struct UserData *>(userp);
    //assert(pstream);
    if (pstream->ss.eof())
    {
        //也是因为移动到末尾时tellg()返回的数据总是不对
        return 0;
    }
    size_t before = pstream->ss.tellg();
    pstream->ss.read((char*)ptr, size*nmemb);
    size_t after = pstream->ss.tellg();
    if (pstream->ss.eof())
    {
        //对std::stringstream不熟悉，不知道为什么移动到末尾后
        //tellg()返回的数据总是不对
//        std::cout<<(pstream->total - before)<<std::endl;
        return pstream->total - before;
    }
//    std::cout<<(after - before)<<std::endl;
    return after - before;
}



int SmtpUtil::SendMail(std::string title,std::string content ,std::string pathfile)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;

    struct upload_status upload_ctx;

      upload_ctx.lines_read = 0;


    mimetic::MultipartMixed head;
    head.header().from(this->_email_address.c_str());
    head.header().to(this->_email_to.c_str());

    if (!this->_email_cc.empty()){
    head.header().cc(this->_email_cc.c_str());

    }

//    DateUtil dateutil;
//    std::string date = dateutil.GetTodayString();
//	std::string title =date;
//	title.append("-主力合约统计表");
    head.header().subject(title);

    head.header().push_back(mimetic::Field("Mime-Version","1.0"));
    head.header().push_back(mimetic::Field("Message-ID","<dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>"));
//    head.body().assign("-------------------------->");



    mimetic::MimeEntity* pMe = new mimetic::MimeEntity;

    pMe->header().push_back(mimetic::Field("Content-Type: application/octet-stream; name=Maincontract.xls"));
    pMe->header().push_back(mimetic::Field("Content-Disposition : attachment; filename=Maincontract.xls"));
    pMe->header().push_back(mimetic::Field("Content-Transfer-Encoding","base64"));

//    head.body().assign("hello there!");
//    FILE *pfile = fopen("/home/vnpy/test1.xls","rb");
    FILE *pfile = fopen(pathfile.c_str(),"rb");
    char buffer[BUFFERR_SIZE];
    uint32_t totalreadbytes = 0;
    while (!feof(pfile))
    {
    	uint32_t readbytes = fread(buffer, 1, BUFFERR_SIZE - 1, pfile);
        if (ferror(pfile) || readbytes == 0)
        {
            break;
        }
        totalreadbytes += readbytes;
        mimetic::Base64::Encoder b64;
        std::stringstream temp;
        std::ostreambuf_iterator<char> out(temp);
        buffer[readbytes] = '\0';
        mimetic::code(buffer,  buffer + readbytes, b64, out);
        std::string str = temp.str();
        pMe->load(str.begin(), str.end(), mimetic::imNone);
    }



    head.body().parts().push_back(pMe);

    //添加一段正文
	 mimetic::MimeEntity* pMe2 = new mimetic::MimeEntity;
	 //说明块是一段文字，文字使用GB2312字符集
	 pMe2->header().push_back(mimetic::Field("Content-Type: text/plain; charset=utf-8"));
//         pMe2->header().push_back(mimetic::Field("Content-Transfer-Encoding","base64"));
	 //把这一块加入到整个邮件中
	 pMe2->body().assign(content);

	 head.body().parts().push_back(pMe2);


//    std::cout<<head;


    struct UserData ud;
    ud.ss<<head;
    ud.ss.seekg(0, std::ios::end);
    ud.total = ud.ss.tellg();
    ud.ss.seekg(0, std::ios::beg);


    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver. Note the use of port 587 here,
        * instead of the normal SMTP port (25). Port 587 is commonly used for
        * secure mail submission (see RFC4403), but you should use whatever
        * matches your server configuration. */
        curl_easy_setopt(curl, CURLOPT_URL, this->_smtp_address.c_str());//"smtp://mainserver.example.net:25");

        /* In this example, we'll start with a plain text connection, and upgrade
        * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
        * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
        * will continue anyway - see the security discussion in the libcurl
        * tutorial for more details. */
        //curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        /* If your server doesn't have a valid certificate, then you can disable
        * part of the Transport Layer Security protection by setting the
        * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
        *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        * That is, in general, a bad idea. It is still better than sending your
        * authentication details in plain text though.
        * Instead, you should get the issuer certificate (or the host certificate
        * if the certificate is self-signed) and add it to the set of certificates
        * that are known to libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See
        * docs/SSLCERTS for more information.
        */
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

        /* A common reason for requiring transport security is to protect
        * authentication details (user names and passwords) from being "snooped"
        * on the network. Here is how the user name and password are provided: */

        curl_easy_setopt(curl, CURLOPT_USERNAME, this->_email_address.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD,this->_email_password.c_str());

        /* value for envelope reverse-path */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, this->_email_address.c_str());
        /* Add two recipients, in this particular case they correspond to the
        * To: and Cc: addressees in the header, but they could be any kind of
        * recipient. */
        recipients = curl_slist_append(recipients, this->_email_to.c_str());

        if (!this->_email_cc.empty()){
        	recipients = curl_slist_append(recipients, this->_email_cc.c_str());
        }
//
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* In this case, we're using a callback function to specify the data. You
        * could just use the CURLOPT_READDATA option to specify a FILE pointer to
        * read from.
        */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, this->my_read);
        curl_easy_setopt(curl, CURLOPT_READDATA, &ud);

//      curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//      curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);

//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//           curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
           curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Since the traffic will be encrypted, it is very useful to turn on debug
        * information within libcurl to see what is happening during the transfer.
        */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);   // 调试用
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        /* send the message (including headers) */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));


        /* free the list of recipients and clean up */
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    return 0;
}




int SmtpUtil::SendMail_Content(std::string title,std::string content )
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;

    struct upload_status upload_ctx;

      upload_ctx.lines_read = 0;


    mimetic::MultipartMixed head;
    head.header().from(this->_email_address.c_str());
    head.header().to(this->_email_to.c_str());

    if (!this->_email_cc.empty()){
    head.header().cc(this->_email_cc.c_str());

    }

//    DateUtil dateutil;
//    std::string date = dateutil.GetTodayString();
//	std::string title =date;
//	title.append("-主力合约统计表");
    head.header().subject(title);

    head.header().push_back(mimetic::Field("Mime-Version","1.0"));
    head.header().push_back(mimetic::Field("Message-ID","<dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>"));
//    head.body().assign("-------------------------->");

    //添加一段正文
	 mimetic::MimeEntity* pMe2 = new mimetic::MimeEntity;
	 //说明块是一段文字，文字使用GB2312字符集
	 pMe2->header().push_back(mimetic::Field("Content-Type: text/plain; charset=utf-8"));
//         pMe2->header().push_back(mimetic::Field("Content-Transfer-Encoding","base64"));
	 //把这一块加入到整个邮件中
	 pMe2->body().assign(content);

	 head.body().parts().push_back(pMe2);


//    std::cout<<head;


    struct UserData ud;
    ud.ss<<head;
    ud.ss.seekg(0, std::ios::end);
    ud.total = ud.ss.tellg();
    ud.ss.seekg(0, std::ios::beg);


    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver. Note the use of port 587 here,
        * instead of the normal SMTP port (25). Port 587 is commonly used for
        * secure mail submission (see RFC4403), but you should use whatever
        * matches your server configuration. */
        curl_easy_setopt(curl, CURLOPT_URL, this->_smtp_address.c_str());//"smtp://mainserver.example.net:25");

        /* In this example, we'll start with a plain text connection, and upgrade
        * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
        * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
        * will continue anyway - see the security discussion in the libcurl
        * tutorial for more details. */
        //curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        /* If your server doesn't have a valid certificate, then you can disable
        * part of the Transport Layer Security protection by setting the
        * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
        *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        * That is, in general, a bad idea. It is still better than sending your
        * authentication details in plain text though.
        * Instead, you should get the issuer certificate (or the host certificate
        * if the certificate is self-signed) and add it to the set of certificates
        * that are known to libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See
        * docs/SSLCERTS for more information.
        */
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

        /* A common reason for requiring transport security is to protect
        * authentication details (user names and passwords) from being "snooped"
        * on the network. Here is how the user name and password are provided: */
        curl_easy_setopt(curl, CURLOPT_USERNAME, this->_email_address.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD,this->_email_password.c_str());

        /* value for envelope reverse-path */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, this->_email_address.c_str());
	   /* Add two recipients, in this particular case they correspond to the
	   * To: and Cc: addressees in the header, but they could be any kind of
	   * recipient. */
	   recipients = curl_slist_append(recipients, this->_email_to.c_str());

       if (!this->_email_cc.empty()){
       	recipients = curl_slist_append(recipients, this->_email_cc.c_str());
       }

       curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* In this case, we're using a callback function to specify the data. You
        * could just use the CURLOPT_READDATA option to specify a FILE pointer to
        * read from.
        */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, this->my_read);
        curl_easy_setopt(curl, CURLOPT_READDATA, &ud);

//      curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//      curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);

//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//           curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
           curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Since the traffic will be encrypted, it is very useful to turn on debug
        * information within libcurl to see what is happening during the transfer.
        */
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  //调试 屏幕打印

        /* send the message (including headers) */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));


        /* free the list of recipients and clean up */
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    return 0;
}
