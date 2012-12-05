#include "../include/JSONHandler/ALRPCObjects/V2/GenericResponse_response.h"
#include "GenericResponse_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
GenericResponse_response& GenericResponse_response::operator =(const GenericResponse_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


GenericResponse_response::~GenericResponse_response(void)
{
  if(info)
    delete info;
}


GenericResponse_response::GenericResponse_response(const GenericResponse_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool GenericResponse_response::checkIntegrity(void)
{
  return GenericResponse_responseMarshaller::checkIntegrity(*this);
}


GenericResponse_response::GenericResponse_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      info(0)
{
}



bool GenericResponse_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool GenericResponse_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool GenericResponse_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void GenericResponse_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool GenericResponse_response::get_success(void) const
{
  return success;
}

const Result& GenericResponse_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* GenericResponse_response::get_info(void) const 
{
  return info;
}

