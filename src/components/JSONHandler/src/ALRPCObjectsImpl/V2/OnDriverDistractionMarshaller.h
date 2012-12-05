#ifndef NSAPPLINKRPCV2_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnDriverDistractionMarshaller
  {
    static bool checkIntegrity(OnDriverDistraction& e);
    static bool checkIntegrityConst(const OnDriverDistraction& e);
  
    static bool fromString(const std::string& s,OnDriverDistraction& e);
    static const std::string toString(const OnDriverDistraction& e);
  
    static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
    static Json::Value toJSON(const OnDriverDistraction& e);
  };
}

#endif
