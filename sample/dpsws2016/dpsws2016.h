#include <pgen2.h>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

struct dpsws2016_s {
  std::string desc;
  uint8_t type; /* 10:digital, 16:hex, 2:bit*/
  uint16_t fmt_length; /* byte */
  uint8_t* msg; /* raw data, without regard type */
  uint16_t msg_length; /* byte */
  std::string text; /* strings regard type */
};

class dpsws2016_header : public pgen::header {
 public:
  static const size_t min_length = 1;
  static const size_t max_length = 10000;
  struct dpsws2016_s dpsws2016_s_list[max_length];
  uint8_t* all_msg = NULL;
  uint16_t all_msg_length = 0;

 public:
  void clear() override
  {
    for(int i = 0; i < max_length; i++ ){
      dpsws2016_s_list[i].desc = "";
      dpsws2016_s_list[i].type = 0;
      dpsws2016_s_list[i].fmt_length = 0;
      if( dpsws2016_s_list[i].msg != NULL ){
	free(dpsws2016_s_list[i].msg);  
      }
      dpsws2016_s_list[i].msg = NULL;
      dpsws2016_s_list[i].msg_length = 0;
      dpsws2016_s_list[i].text = "";
    }
    if( all_msg != NULL ){
      free(all_msg);
    }
    all_msg = NULL;
    all_msg_length = 0;
  }

  void summary(bool moreinfo=false) const override
  {
    printf("DPSWS2016\n");
  }

  void write(void* buffer, size_t bufferlen) const override
  {
    if (bufferlen < min_length) {
      throw pgen::exception("pgen::dpsws2016_header::write: buflen is too small");
    }

    uint8_t* buffer_point = reinterpret_cast<uint8_t*>(buffer);

    for(int i = 0; i < max_length; i++ ){
      if( dpsws2016_s_list[i].type != 0 ){
	memcpy(buffer_point, dpsws2016_s_list[i].msg, dpsws2016_s_list[i].msg_length);
	buffer_point += dpsws2016_s_list[i].msg_length;
	bufferlen    -= dpsws2016_s_list[i].msg_length;
      }else{
	break;
      }
    }
  }

  void read(const void* buffer, size_t bufferlen ) override
  {
    char cbuf[16];

    if (bufferlen < min_length) {
      throw pgen::exception("pgen::dpsws2016_header::read: buflen is too small");
    }

    all_msg_length = bufferlen;
    const uint8_t* buffer_point = reinterpret_cast<const uint8_t*>(buffer);
    all_msg = (uint8_t*)malloc(sizeof(uint8_t)*all_msg_length);
    memcpy(all_msg, buffer_point, all_msg_length);

    for(int i = 0; i < max_length; i++ ){
      if( bufferlen > 0 ){
        if( dpsws2016_s_list[i].fmt_length > 0 ){
          dpsws2016_s_list[i].msg_length = dpsws2016_s_list[i].fmt_length;
        }else{
          dpsws2016_s_list[i].msg_length = bufferlen;
        }
        dpsws2016_s_list[i].msg = (uint8_t*)malloc(sizeof(uint8_t)*dpsws2016_s_list[i].msg_length);
	memset(dpsws2016_s_list[i].msg, 0, dpsws2016_s_list[i].msg_length);
	memcpy(dpsws2016_s_list[i].msg, buffer_point, dpsws2016_s_list[i].msg_length);
	dpsws2016_s_list[i].text = "";
	if( dpsws2016_s_list[i].type == 2 ){
          for( int j = 0; j <  dpsws2016_s_list[i].msg_length; j++){
	    sprintf(cbuf, "%08b ", dpsws2016_s_list[i].msg[j]);
            dpsws2016_s_list[i].text.append(cbuf);
          }
	}else if( dpsws2016_s_list[i].type == 8 ){
          for( int j = 0; j <  dpsws2016_s_list[i].msg_length; j++){
	    sprintf(cbuf, "%04o ", dpsws2016_s_list[i].msg[j]);
            dpsws2016_s_list[i].text.append(cbuf);
          }
	}else if(dpsws2016_s_list[i].type == 10 ){
          for( int j = 0; j <  dpsws2016_s_list[i].msg_length; j++){
	    sprintf(cbuf, "%03d ",  dpsws2016_s_list[i].msg[j]);
            dpsws2016_s_list[i].text.append(cbuf);
          }
	}else if(dpsws2016_s_list[i].type == 16 ){
          for( int j = 0; j <  dpsws2016_s_list[i].msg_length; j++){
            sprintf(cbuf, "%02x ",  dpsws2016_s_list[i].msg[j]);
            dpsws2016_s_list[i].text.append(cbuf);
          }
	}else{
          for( int j = 0; j <  dpsws2016_s_list[i].msg_length; j++){
            sprintf(cbuf, "%02x ",  dpsws2016_s_list[i].msg[j]);
            dpsws2016_s_list[i].text.append(cbuf);
          }
	}
	buffer_point += dpsws2016_s_list[i].msg_length;
	bufferlen    -= dpsws2016_s_list[i].msg_length;
      }else{
	break;
      }
    }
  }

  size_t length() const override
  {
    return all_msg_length;
  }

  std::string print_str()
  {
    char cbuf[1024];
    std::string ret_str = "";

    ret_str.append("\n");
    ret_str.append("Print (DPSWS2016)\n\n");
    sprintf(cbuf, "min_length = %d\n", min_length);
    ret_str.append(cbuf);
    sprintf(cbuf,"max_length = %d\n\n", max_length);
    ret_str.append(cbuf);

    ret_str.append("msg part list\n\n");
    for(int i = 0; i < max_length; i++ ){
      if( dpsws2016_s_list[i].type != 0 ){
        sprintf(cbuf,"msg_fmt_id = %d\n", i);
        ret_str.append(cbuf);
        ret_str.append("desc       = ");
        ret_str.append(dpsws2016_s_list[i].desc);
        ret_str.append("\n");
        if( dpsws2016_s_list[i].type == 2 ){
          sprintf(cbuf,"type       = bit\n");
        }else if( dpsws2016_s_list[i].type == 8 ){
          sprintf(cbuf,"type       = oct\n");
        }else if( dpsws2016_s_list[i].type == 10 ){
          sprintf(cbuf,"type       = dig\n");
        }else if( dpsws2016_s_list[i].type == 16 ){
          sprintf(cbuf,"type       = hex\n");
        }else{
          sprintf(cbuf,"type       = %d\n", dpsws2016_s_list[i].type);
        }
        ret_str.append(cbuf);
        sprintf(cbuf,"fmt_length = %d\n", dpsws2016_s_list[i].fmt_length);
        ret_str.append(cbuf);
        if( dpsws2016_s_list[i].msg_length > 0 ){
          sprintf(cbuf,"msg_length = %d\n", dpsws2016_s_list[i].msg_length);
          ret_str.append(cbuf);
          ret_str.append("msg(hex)   = ");
          for(int j = 0; j < dpsws2016_s_list[i].msg_length ; j++ ){
            sprintf(cbuf,"%02x ", dpsws2016_s_list[i].msg[j]);
            ret_str.append(cbuf);
          }
          ret_str.append("\n");
/*        ret_str.append("text   = ");
          ret_str.append(dpsws2016_s_list[i].text);
          ret_str.append("\n");
*/      }
        ret_str.append("\n");
      }else{
        break;
      }
    }
    ret_str.append("\n");

    if( all_msg_length > 0 ){
      sprintf(cbuf,"all_msg_length = %d\n", all_msg_length);
      ret_str.append(cbuf);
      ret_str.append("all_msg (hex)  = ");
      for(int j = 0; j < all_msg_length; j++ ){
        sprintf(cbuf,"%02x ", all_msg[j]);
        ret_str.append(cbuf);
      }
      ret_str.append("\n\n");
    }
    return(ret_str);
  }

};

class dpsws2016 : public pgen::packet {
 private:
  void init_headers() override
  {
    headers = {&DPSWS2016};
  }

 public:

  dpsws2016_header DPSWS2016;

  dpsws2016()
  {
    clear();
    init_headers();
  }

  dpsws2016(const void* buffer, size_t bufferlen) : dpsws2016()
  {
    analyze(buffer, bufferlen);
  }

  dpsws2016(const dpsws2016& rhs) : dpsws2016()
  {
    DPSWS2016 = rhs.DPSWS2016;
    init_headers();
  }

  void clear() override
  {
    DPSWS2016.all_msg = NULL;
    DPSWS2016.all_msg_length = 0;
  
    uint8_t mbuf[DPSWS2016.max_length];
    std::string sbuf = "";
    std::string tbuf = "";
    char cbuf[16];
    uint16_t nlen = 0;

    for(int i = 0; i < DPSWS2016.max_length; i++ ){
      DPSWS2016.dpsws2016_s_list[i].type = 0;

      sbuf = "";
      std::getline(std::cin, sbuf);
      tbuf = "";
      for( const auto c : sbuf ){
        if( c != '\r' && c != '\n' && c != '\0' ){
          tbuf += c;
        }
      }
      sbuf = std::move(tbuf);

      if( sbuf.length() > 0 ){
        std::string ebuf;
        std::stringstream strstr(sbuf);

        std::getline(strstr, ebuf, ':');
        DPSWS2016.dpsws2016_s_list[i].desc = std::string(ebuf);

        DPSWS2016.dpsws2016_s_list[i].type = 0;
        std::getline(strstr, ebuf, ':');
        if( ebuf == "bit" ){
          DPSWS2016.dpsws2016_s_list[i].type = 2;
        }else if( ebuf == "oct" ){
          DPSWS2016.dpsws2016_s_list[i].type = 8;
        }else if( ebuf == "dig" ){
          DPSWS2016.dpsws2016_s_list[i].type = 10;
        }else if( ebuf == "hex" ){
          DPSWS2016.dpsws2016_s_list[i].type = 16;
        }else{
          sscanf(ebuf.c_str(), "%d", &(DPSWS2016.dpsws2016_s_list[i].type));
        }

        std::getline(strstr, ebuf, ':');
        sscanf(ebuf.c_str(), "%d", &(DPSWS2016.dpsws2016_s_list[i].fmt_length));
        if( DPSWS2016.dpsws2016_s_list[i].fmt_length >= 65535 ){
          DPSWS2016.dpsws2016_s_list[i].fmt_length = 0;
        }

        ebuf = "";
        std::getline(strstr, ebuf, ':');

        DPSWS2016.dpsws2016_s_list[i].text = "";
        DPSWS2016.dpsws2016_s_list[i].msg = NULL;
        if( ebuf.length() > 0 ){
          DPSWS2016.dpsws2016_s_list[i].text = std::string(ebuf);

          std::stringstream bstr(ebuf);
          std::string bbuf = "";
          nlen = 0;
          while( std::getline(bstr, bbuf, ' ') ){
            if( bbuf.length() > 0 ){
              if( DPSWS2016.dpsws2016_s_list[i].fmt_length > 0 ){
                if( nlen >= DPSWS2016.dpsws2016_s_list[i].fmt_length ){
                  break;
                }
              }
              if( DPSWS2016.dpsws2016_s_list[i].type == 2 ){
                sscanf(bbuf.c_str(), "%b", &(mbuf[nlen]));
              }else if( DPSWS2016.dpsws2016_s_list[i].type == 8 ){
                sscanf(bbuf.c_str(), "%o", &(mbuf[nlen]));
              }else if( DPSWS2016.dpsws2016_s_list[i].type == 10 ){
                sscanf(bbuf.c_str(), "%d", &(mbuf[nlen]));
              }else if( DPSWS2016.dpsws2016_s_list[i].type == 16 ){
                sscanf(bbuf.c_str(), "%x", &(mbuf[nlen]));
              }else{
                sscanf(bbuf.c_str(), "%x", &(mbuf[nlen])); 
              }
              nlen++;
            }
            bbuf = "";
          }

          DPSWS2016.dpsws2016_s_list[i].msg_length = nlen;
          DPSWS2016.dpsws2016_s_list[i].msg = (uint8_t*)malloc(sizeof(uint8_t)*nlen);
          DPSWS2016.dpsws2016_s_list[i].msg_length = nlen;
          memcpy(DPSWS2016.dpsws2016_s_list[i].msg, mbuf, nlen);
          DPSWS2016.all_msg_length += nlen;
          nlen = 0;
        }
      }else{
        break;
      }
      sbuf = "";
    }
    DPSWS2016.all_msg = (uint8_t*)malloc(sizeof(uint8_t)*DPSWS2016.all_msg_length);
    nlen = 0;
    for(int i = 0; i < DPSWS2016.max_length; i++ ){
      if( DPSWS2016.dpsws2016_s_list[i].type != 0 ){
        memcpy(DPSWS2016.all_msg + nlen, DPSWS2016.dpsws2016_s_list[i].msg, DPSWS2016.dpsws2016_s_list[i].msg_length);
        nlen += DPSWS2016.dpsws2016_s_list[i].msg_length;
      }else{
        break;
      }
    }
  }

};

bool is_dpsws2016_packet(const void* _buf, size_t len )
{
    return true;
}

