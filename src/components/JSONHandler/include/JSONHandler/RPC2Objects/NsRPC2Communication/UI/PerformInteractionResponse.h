#ifndef NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TriggerSource.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class PerformInteractionResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      PerformInteractionResponse(const PerformInteractionResponse& c);
      PerformInteractionResponse(void);
    
      PerformInteractionResponse& operator =(const PerformInteractionResponse&);
    
      virtual ~PerformInteractionResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const unsigned int* get_choiceID(void);
      const NsAppLinkRPC::TriggerSource* get_triggerSource(void);

// setters
/// choiceID <= 2000000000
      bool set_choiceID(const unsigned int& choiceID);

      void reset_choiceID(void);

      bool set_triggerSource(const NsAppLinkRPC::TriggerSource& triggerSource);

      void reset_triggerSource(void);


    private:

      friend class PerformInteractionResponseMarshaller;

      unsigned int* choiceID;
      NsAppLinkRPC::TriggerSource* triggerSource;

    };
  }
}

#endif
